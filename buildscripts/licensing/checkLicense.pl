#!/usr/bin/perl
use strict;

# This script scans the codebase for .c, .cpp, .h, .hh, .cxx, .clj, and .java
# files and tries to determine what if any license header they have. It can
# recognize BSD (no distinction between two- and three-clause though), LGPL,
# and Apache, and will otherwise mark the file as no-known-license.
#
# Usage: run with no arguments at the root of the source repository (or root
# of whatever section of the repository you want to check).

# Find files with a given file extension, and try to figure out their licenses.
sub checkFiles {
    my ($filetype, $bad, $good) = @_;
    foreach my $file (`find . -name "*$filetype"`) {
        chomp $file;
        my $license = scanForLicense($file, $bad, $good);
        print "$file: $license\n";
    }
}

# Try to figure out what license the specified file uses, using the specified
# "bad" and "good" comment styles. We find the first comment block(s)
# (with multiple blocks, they can only be separated by whitespace) and scan
# them for words or phrases that indicate specific licenses.
sub scanForLicense {
    my ($file, $badCommentStyle, $goodCommentStyle) = @_;
    my $didStartComments = 0;
    foreach my $line (`cat "$file"`) {
        if ($line =~ /^\s*$badCommentStyle/ or
            $line =~ /^\s*$goodCommentStyle/) {
            # In a comment block.
            $didStartComments = 1;
            my $license = getLicense($line);
            if (defined $license) {
                # Found license info for this file.
                if ($line =~ /^\s*$goodCommentStyle/) {
                    return "Fixed " . $license;
                }
                else {
                    return "Unfixed " . $license;
                }
            }
        }
        elsif ($line !~ /^\s*$/ and $didStartComments) {
            # Finished the first comment block without finding a license.
            return "Unrecognized";
        }
    }
    # Finished the file without finding any comments.
    return "Unrecognized";
}

# Scan the provided text string for license type indicators.
sub getLicense {
    my ($line) = @_;
    if ($line =~ /Lesser General/ or $line =~ /Lesser GPL/
            or $line =~ /LGPL/) {
        return "LGPL";
    }
    elsif ($line =~ /General Public/ or $line =~ /GPL/) {
        return "GPL";
    }
    elsif ($line =~ /Apache/) {
        return "Apache";
    }
    elsif ($line =~ /Redistributions in binary form must reproduce/ or
            $line =~ /Redistributions of source code must retain the above copyright notice/ or
            $line =~ /BSD license/) {
        # That last one is not technically a valid BSD license, but the intent
        # is clear.
        return "BSD";
    }
    # Nothing recognized.
    return undef;
}

# Pattern for matching C-style comments used for our old, incorrect license
# text headers.
my $badCComment = "\\/\\/";
# Pattern for matching C-style comments used for fixed files.
my $goodCComment = " \\*";
#print "C files:\n";
checkFiles("\\.c", $badCComment, $goodCComment);
#print "C++ files:\n";
checkFiles("\\.cpp", $badCComment, $goodCComment);
checkFiles("\\.h", $badCComment, $goodCComment);
checkFiles("\\.hh", $badCComment, $goodCComment);
#print "CXX files:\n";
checkFiles("\\.cxx", $badCComment, $goodCComment);
#print "Clojure files:\n";
checkFiles("\\.clj", "we don't have a way to differentiate good clojure files from bad ones", ";");
#print "Java files:\n";
checkFiles("\\.java", $badCComment, $goodCComment);
#build files
checkFiles("\\.m4", "we can't tell apart bad and good m4 files either", "#");
