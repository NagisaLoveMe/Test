#!/usr/local/bin/perl
#use warnings;
#use diagnostics;
#use POSIX qw(strftime);

if (!$ARGV[0] || !$ARGV[1] || !$ARGV[2]|| !$ARGV[3]|| !$ARGV[4]|| !$ARGV[5]|| !$ARGV[6])
{
	die "Param:[inputfile] [function header] [function tail] [outputfile name] [output scatter file name] [function header check] [dll pool check]";
}

my $inputSymFileName = $ARGV[0];
my $functionHeader = $ARGV[1];
my $functionTail = $ARGV[2];
my $outputFileName = $ARGV[3];
my $outputScatFileName = $ARGV[4];
my $functionHeaderChcek = $ARGV[5];
my $poolAddrChcek = $ARGV[6];

my $isInScope = 0;

my $addressOfPool;

if(-e $outputFileName)
{
	print("$outputFileName exist, delete it\n");
	unlink("$outputFileName"); 
}

($sec,$min,$hour,$mday,$mon,$year_off,$wday,$yday,$isdat) = localtime();
$year_off += 1900;
$mon += 1;
open(FILE_HANDLE_OUT, ">$outputFileName") or die "cannot open $outputFileName\n";
print(FILE_HANDLE_OUT "#<SYMDEFS># OpenAT SYM Generate, Last Updated: $year_off-$mon-$mday $hour:$min:$sec\n");

if(-e $inputSymFileName)
{ #source file exist? 
	print("file exist.\n"); 
	open(FILE_HANDLE_IN, "$inputSymFileName");
	my @src_context = <FILE_HANDLE_IN>;
	foreach $eachline (@src_context) 
	{
		#print "$eachline";
		if($eachline =~ m/(0x[0-9A-Fa-f]{8}) [TADN] (\w+)/i)
		{
			#print "GET ONE LINE: $1\n";
			if($2 eq "openat_dll_pool"){
				print "WE GET ONE LINE: openat_dll_pool\n";
				if($1 ne $poolAddrChcek)
				{
					die"the address of openat_dll_pool Illegal! $1:$poolAddrChcek\n"
				}
				else
				{
					$addressOfPool = $1;
				}
			}
			
			if($2 eq $functionHeader){
				$isInScope = 1;
				print "WE GET function Header.\n";
				if($1 ne $functionHeaderChcek)
				{
					die"the address of $functionHeader Illegal! $1:$functionHeaderChcek\n"
				}
			}
			
			if($2 eq $functionTail){
				$isInScope = 0;
				print "WE GET function Tail.\n";
				print(FILE_HANDLE_OUT "$eachline");
			}
		
			if($isInScope){
				print(FILE_HANDLE_OUT "$eachline");
			}
		}
		close(FILE_HANDLE_IN);
	}
}
else
{ 
	print("file not exist\n");
} 

print("Let me see the address of pool address: $addressOfPool\n");

close(FILE_HANDLE_OUT);


#generate the scatter file
if(-e $outputScatFileName)
{
	print("$outputScatFileName exist, delete it\n");
	unlink("$outputScatFileName"); 
}
open(FILE_HANDLE_OUT, ">$outputScatFileName") or die "cannot open $outputScatFileName\n";
print(FILE_HANDLE_OUT &scatter_file_body());
close(FILE_HANDLE_OUT);
print("$outputScatFileName generated successfully!\n");

sub scatter_file_body
{
    my $template = <<"__TEMPLATE";
;/*****************************************************************************
; *
; * Filename:
; * ---------
; *   $outputScatFileName
; *
; * Project:
; * --------
; *   OPEN CPU 60D
; *
; * Description:
; * ------------
; *   defines the memory map for the validation board
; *
; * Author:
; * -------
; *   system auto generator v0.1beta
; *
; * Auto Generated Date:
; * -------
; *   $year_off/$mon/$mday $hour:$min:$sec
; *
; * Revision History: None
; ****************************************************************************/
;
;--------------------
;=========================================================================
; This scatter file is auto-generated and please DO NOT change the content at will
;=========================================================================
;
;--------------------
; SCHEME   : external 3.75MB flash memory and 4MB SRAM
; PLATFORM : MT6260D
; FEATURES : ESBNOR 
;
;--------------------

;$addressOfPool is the address of openat_dll_pool, DO NOT modify it.
;if the position of openat_dll_pool changed, system will generate a new one.
;size can be extended, now is 1024k(0x100000)

OA_ALL $addressOfPool 0x40000
{
    OA_CODE $addressOfPool 0x40000
    {
         *.o (OA_USER_ENTRY,+First)
         * (+RO,+ZI,+RW)
    }
}

__TEMPLATE

	return $template;
}
