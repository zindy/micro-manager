#!/usr/bin/perl
use strict;

# This script fixes the license block at the top of the specified file(s).
# Certain existing metadata tags (COPYRIGHT, SUBSYSTEM, AUTHOR, NOTES,
# DESCRIPTION) will be preserved (or at least an attempt will be made), but
# others will be destroyed. The file will be replaced by its updated version,
# and the original will be copied to $file.orig.
# Usage:
# fixLicense.pl newlicense oldlicense file
# newlicense: the desired new license type. May be "BSD" or "LGPL"
# oldlicense: what we think the license text is. Only really relevant if the
# license is "Unrecognized", in which case we do not look for a comment block
# at the top of the file.

use File::Copy;

my $author = "AUTHOR";
my $notes = "NOTE";
my @importantTags = ("DESCRIPTION", $author, $notes, "COPYRIGHT");

# Text for the BSD license. Taken from
# http://opensource.org/licenses/BSD-3-Clause
my $BSD = <<EOT
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
EOT
;

# Text for the LGPL license.
# Adapted from http://www.gnu.org/licenses/gpl-howto.html
my $LGPL = <<EOT
This is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
EOT
;

# Scan the file for the tags we care about and return a hashmap of them.
sub extractTags {
    my ($file, $comment, @lines) = @_;
    # Figure out where the comment blocks is/are. HACK: assume the first
    # comment block is the one we need to replace.
    my $haveStarted = 0;
    my $lineNum = 0;
    my $tags;
    foreach my $line (@lines) {
        if ($line =~ /^\s*$comment/ and not $haveStarted) {
            $haveStarted = 1;
            $tags->{"start"} = $lineNum;
        }
        elsif ($line !~ /^\s*$comment/ and $line !~ /^\s*$/) {
            # Non-comment, non-blank line; end of block.
            $tags->{"end"} = $lineNum;
            last;
        }
        $lineNum++;
    }
    my @trimmedLines = @lines[$tags->{"start"} .. $tags->{"end"}];

    # Read each tag from the file.
    foreach my $tag (@importantTags) {
        my $taglines = extractTag($tag, $comment, @trimmedLines);
        # Only record tags if they have any content.
        if (@$taglines) {
            $tags->{$tag} = $taglines;
        }
    }
    return $tags;
}

# Scan the file for the specified tag and return the lines associated with it,
# stripping off the leading comment and whitespace as we go.
sub extractTag {
    my ($tag, $comment, @lines) = @_;
    my $result = [];
    my $amReading = 0;
    # Allow for plural tag identifiers (e.g. AUTHORS or NOTES)
    my $pattern = $tag . "S?";
    foreach my $line (@lines) {
        if ($line =~ /$pattern/) {
            # Recognized tag; start reading its contents.
            $amReading = 1;
            $line =~ s/$pattern://;
        }
        elsif ($amReading and
            ($line !~ /^\s*$comment/ or
             $line =~ /[A-Z]+:/ or $line =~ /^\s*$/)) {
            # Found another tag or a blank line, or ran out of comment.
            last;
        }
        # Ignore lines that are just spanning the line with ---- or /////
        if ($amReading and $line !~ /$comment-+$/ and
                $line !~ /$comment\/+$/) {
            # Strip out the leading comment and any following whitespace.
            $line =~ s/^$comment//;
            if ($tag !~ /$notes/) {
                # Notes have their whitespace left as-is.
                $line =~ s/^\s*//;
            }
            if ($line !~ /^\s*$/) {
                push @$result, $line;
            }
        }
    }
    return $result;
}

# Print the provided license text to the provided file handle, preceded by the
# provided comment string.
sub printLicense {
    my ($fh, $comment, $text) = @_;
    foreach my $line (split /\n/, $text) {
        if ($line =~ /^\s*$/) {
            # Blank line; ignore whitespace.
            print $fh "$comment\n";
        }
        else {
            print $fh "$comment $line\n";
        }
    }
}

# Start or end a block comment.
sub blockComment {
    my ($type, $isStart) = @_;
    if ($type eq "C++") {
        return $isStart ? "/*\n" : " */\n\n";
    }
    return "";
}

my $license = $ARGV[0];
my $oldlicense = $ARGV[1];
my $file = $ARGV[2];
copy($file, "$file.orig") or die "Unable to make backup of file $file";
print "Applying $license to $file\n";

# Determine comment string and file type.
my $type = "C++";
my $comment = "\/\/";
if ($file =~ /\.clj$/) {
    # Clojure file.
    $type = "Clojure";
    $comment = ";";
}
elsif ($file =~ /\.m4$/) {
    # M4 file.
    $type = "m4";
    $comment = "#";
}

# Read the file into memory.
open my $handle, '<', $file;
chomp (my @lines = <$handle>);
close $handle;

my $tags;
my $start = 0;
my $end = 0;
if ($oldlicense ne "Unrecognized") {
    # Extract information about the comment block.
    $tags = extractTags($file, $comment, @lines);
    $start = $tags->{"start"};
    $end = $tags->{"end"};
    delete $tags->{"start"};
    delete $tags->{"end"};
}

# Determine text for middle of comment blocks.
my $midComment = " *";
if ($type eq "Clojure") {
    $midComment = ";";
}
# Replace the existing comment block with a new one.
open(my $fh, ">$file") or die "Couldn't open $file for writing";
my $lineNum = 0;
foreach my $line (@lines) {
    if ($lineNum == $start) {
        print $fh blockComment($type, 1);

        #Print the tags.
        foreach my $tag (@importantTags) {
            if (not exists $tags->{$tag}) {
                # Don't have this tag.
                next;
            }
            if ($tag eq $notes) {
                # Do the notes later.
                next;
            }
            # Only the author tag gets replicated in-text.
            if ($tag eq $author) {
                print $fh $midComment . " $tag:\n";
            }
            foreach my $tagline (@{$tags->{$tag}}) {
                if ($tag eq "COPYRIGHT") {
                    if ($tagline !~ /Regents/) {
                        # Fix UCSF copyright notice.
                        $tagline =~ s/University of California,? San Francisco/Regents of the University of California/;
                        $tagline =~ s/UCSF,?/Regents of the University of California/;
                    }
                    my $dates = "";
                    if ($tagline =~ /(20\d\d)(\s*[-,]\s*20\d\d)?/) {
                        $dates = "$1$2";
                        # Move copyright dates to the front.
                        $tagline =~ s/$dates//;
                    }
                    if ($tagline !~ /^\s*all rights reserved\s*$/i) {
                        $tagline = "Copyright (c) $dates $tagline";
                    }
                    # Remove trailing commas that show up in some files when
                    # listing copyright members.
                    $tagline =~ s/,\s*$//;
                }
                # Remove trailing whitespace.
                $tagline =~ s/\s*$//;
                print $fh "$midComment $tagline\n";
            }
            # Blank line between tags, with no trailing whitespace.
            print $fh "$midComment\n";
        }
        if ($oldlicense eq "Unrecognized") {
            # Insert a Regents of the University copyright notice, as
            # unrecognized licenses automatically have no tags.
            print $fh $midComment . " Copyright (c) 2005-2015 Regents of the University of California\n$midComment\n";
        }

        # Print the license text.
        if ($license eq "BSD") {
            printLicense($fh, $midComment, $BSD);
        }
        elsif ($license eq "LGPL") {
            printLicense($fh, $midComment, $LGPL);
        }
        else {
            print "Unknown license $license\n";
        }

        # Finish the comment block.
        print $fh blockComment($type, 0);

        if (exists $tags->{$notes}) {
            # Insert the notes block.
            print $fh blockComment($type, 1);
            print $fh $midComment . " NOTES:\n";
            foreach my $tagline (@{$tags->{$notes}}) {
                print $fh "$midComment $tagline\n";
            }
            print $fh blockComment($type, 0);
        }
    }
    if ($lineNum < $start or $lineNum >= $end) {
        # Not part of the comment block; copy it over.
        print $fh "$line\n";
    }
    $lineNum++;
}
close($fh);
