# REPORT:Sample report one

# Read in the print data
do "printdata.pl";

# Open the report file
open( REPORTFILE, ">report.txt" ) || die "Can't open report.txt";

foreach $PhotoId ( sort keys %Photos ) {

	$Description = $Photos{$PhotoId}{"Description"};
	$Rating = $Photos{$PhotoId}{"Rating"};
	$Keywords = $Photos{$PhotoId}{"Keywords"};
	write( REPORTFILE );
}

close( REPORTFILE );

print( "Report Completed.\n" );

# Report Format
format REPORTFILE =
@<<<<<<<<  @|||||  ^<<<<<<<<<<<<<<<<<<<<<<<<<<  ^<<<<<<<<<<<<<<<<<<<<<<<<<
$PhotoId,  $Rating,$Description,                $Keywords
~~                 ^<<<<<<<<<<<<<<<<<<<<<<<<<<  ^<<<<<<<<<<<<<<<<<<<<<<<<<
                   $Description,                $Keywords
---------------------------------------------------------------------------
.

format REPORTFILE_TOP =
===========================================================================
Sample Report One
---------------------------------------------------------------------------
Photo Id   Rating  Description                  Keywords           Page @<<
                                                                        $%
===========================================================================
.

format FILM =
===========================================================================
.