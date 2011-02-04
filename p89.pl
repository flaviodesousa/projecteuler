#! /usr/bin/perl

our $saved = 0;

while (<>) {
	next unless /(\w+)/;
	my $original = $1;
	$saved += length($original);
	my $start = $original;
	my $end;
	print "trying $original:\n";
	#while (1) {
		$end = $start;

		while ($end =~ s/IIIII/V/g) {}
		while ($end =~ s/VV/X/g) {}
		while ($end =~ s/XXXXX/L/g) {}
		while ($end =~ s/LL/C/g) {}
		while ($end =~ s/CCCCC/D/g) {}
		while ($end =~ s/DD/M/g) {}
		while ($end =~ s/IIII/IV/g) {}
		while ($end =~ s/XXXX/XL/g) {}
		while ($end =~ s/CCCC/CD/g) {}
		while ($end =~ s/VIV/IX/g) {}
		while ($end =~ s/LXL/XC/g) {}
		while ($end =~ s/DCD/CM/g) {}
	#}
	$saved -= length($end);
	printf "%s -> %s = %d\n", $original, $end, $saved;
}

print "total saved = $saved\n";