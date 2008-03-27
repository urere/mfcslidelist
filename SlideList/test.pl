
do "printdata.pl";

print( "Simple Photo Report\n" );

foreach $PhotoId ( sort keys %Photos ) {
	$Description = $Photos{$PhotoId}{"Description"};
	$Rating = $Photos{$PhotoId}{"Rating"};
	$Keywords = $Photos{$PhotoId}{"Keywords"};
	write( STDOUT );
}


# Report Format
format STDOUT =
@<<<<<<<<  @|||||  ^<<<<<<<<<<<<<<<<<<<<<<<  ^<<<<<<<<<<<<<<<<<<<<<<<
$PhotoId,  $Rating,$Description,             $Keywords
~~                 ^<<<<<<<<<<<<<<<<<<<<<<<  ^<<<<<<<<<<<<<<<<<<<<<<<
                   $Description,             $Keywords
.

format STDOUT_TOP =
===============================================================================
Photo Id   Rating  Description               Keywords
===============================================================================
.