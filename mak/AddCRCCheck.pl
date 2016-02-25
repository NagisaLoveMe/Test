#!/usr/local/bin/perl
#use warnings;
#use diagnostics;
#use POSIX qw(strftime);
#use strict;
use Encode;
use strict;

if (!$ARGV[0])
{
	die "Param:[input bin file]";
}

my $inputBinFile = $ARGV[0];

open(FILE_HANDLE_SRC, $inputBinFile) or die "cannot open $inputBinFile: $!";
binmode(FILE_HANDLE_SRC);

my $CRC16Lo=0x00, my $CRC16Hi=0x00;
my $showCRC16Lo, my $showCRC16Hi;
my $CL=0x01, my $CH=0xa0;

my $SaveHi;
my $SaveLo;
my $I;
my $Flag;

my $read_file;
my $readCounter = 0;

my $binBuffer;
while(read(FILE_HANDLE_SRC, $binBuffer, 1))
{
#my $hex = unpack("H*", $binBuffer);
	my $hex = ord($binBuffer);
	$CRC16Lo = $CRC16Lo ^ $hex; 
	for ($I=0; $I<=7; $I++) 
	{
	    $SaveHi = $CRC16Hi;
	    $SaveLo = $CRC16Lo;

	    $CRC16Hi = $CRC16Hi >> 1;
	    $CRC16Lo = $CRC16Lo >> 1;

	    if (($SaveHi & 0x01) == 0x01)
	    {
	        $CRC16Lo = $CRC16Lo + 0x80;
	    }
	    else
	    {
	        
	    }
	    if (($SaveLo & 0x01)  == 0x01)
	    {
	        $CRC16Hi = $CRC16Hi ^ $CH;
	        $CRC16Lo = $CRC16Lo ^ $CL;
	    }
	}
	$readCounter ++;
}

#$showCRC16Lo = unpack("H*", $CRC16Hi);
#$showCRC16Hi = unpack("H*", $CRC16Lo);
#printf $strBuffer;
printf("File Size is %d\n", $readCounter);
printf("CRC result: 0x%02X:0x%02X\n", $CRC16Lo, $CRC16Hi);
close(FILE_HANDLE_SRC);

open(FILE_HANDLE_SRC, ">>$inputBinFile") or die "cannot open $inputBinFile: $!";
binmode(FILE_HANDLE_SRC);
syswrite(FILE_HANDLE_SRC, pack("L",$CRC16Lo), 1, 0); 
syswrite(FILE_HANDLE_SRC, pack("L",$CRC16Hi), 1, 0); 
close(FILE_HANDLE_SRC);

printf("Add CRC tail OK!");