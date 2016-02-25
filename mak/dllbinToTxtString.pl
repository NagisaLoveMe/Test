#!/usr/local/bin/perl
#use warnings;
#use diagnostics;
#use POSIX qw(strftime);
#use strict;
use Encode;
use strict;

if (!$ARGV[0] || !$ARGV[1] || !$ARGV[2])
{
	die "Param:[Max file len in KB] [input bin file] [output *.c file]";
}

my $inputBinFile = $ARGV[1];
my $outputSrcFile = $ARGV[2];
my $MaxFileLen = $ARGV[0] * 1024;

if(-e $outputSrcFile)
{
	print("$outputSrcFile exist, delete it\n");
	unlink("$outputSrcFile"); 
}

open(FILE_HANDLE_OUT, ">$outputSrcFile") or die "cannot create $outputSrcFile\n";
open(FILE_HANDLE_SRC, $inputBinFile) or die "cannot open $inputBinFile: $!";
binmode(FILE_HANDLE_SRC);

my $binBuffer;
my $strBuffer;
my $read_file;
my $readCounter = 0;

do
{
	$read_file = read(FILE_HANDLE_SRC, $binBuffer, 1);
	#Encode::_utf8_on($binBuffer);
	#Encode::_utf8_off($binBuffer);
	if($read_file)
	{
		my $hex = unpack("H*", $binBuffer);
		my $subStrBuf = sprintf("0x%s, ", $hex);
		$strBuffer .= $subStrBuf;
		$readCounter = $readCounter + 1;
		if(($readCounter%16)==0)
		{
			$strBuffer .= "\n";
			$readCounter = 0;
		}
	}
}while($read_file && ($readCounter<$MaxFileLen));

#printf $strBuffer;
print(FILE_HANDLE_OUT &copyright_file_header());

close(FILE_HANDLE_OUT);
close(FILE_HANDLE_SRC);


sub copyright_file_header
{
    my $template = <<"__TEMPLATE";
/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  MobileTech(Shanghai) Co., Ltd.
 *
 *****************************************************************************/
 //Do not modify, Auto generated for openatdll.bin

#pragma arm section rodata = "LEADING_PART_OA"

const char openatdll[$MaxFileLen] = {\n$strBuffer};

#pragma arm section rodata
__TEMPLATE

	return $template;
}