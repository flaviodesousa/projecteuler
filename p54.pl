#! /usr/bin/perl -W

# A high
# B pair
# C 2pair
# D 3of
# E straight
# F flush
# G fullhand
# H 4of
# I straightflush
# cards normalization:
# A23456789TJQKA
# ABCDEFGHIJKLMN

sub Flush {
	return "" unless join(' ',@_) =~ /.(.) .\1 .\1 .\1 (.)\1/;
	return "F$2"
}

sub Straight {
	my @hand = @_;
	# ace high changed into a 1 if lowest card is a 2 (12345 straight?)
	if ($hand[4] eq 'N' && $hand[0] eq 'B') { 
		$hand[4] = 'A';
		@hand = sort @hand;
	}
	for my $i (0..3) {
		return "" if ord($hand[$i+1]) - ord($hand[$i]) != 1;
	}
	return "E" . $hand[4];
}

sub StraightFlush {
	my @hand = @_;
	return "" unless &Flush(@hand);
	my $straight = &Straight(@hand);
	return "" unless $straight;
	return "I".substr($straight,-1);
}

sub Four {
	return "" unless join(' ', @_) =~ /(.). \1. \1. \1./;
	return "H$1";
}

sub FullHouse {
	my @hand = @_;
	return "" unless join(' ', @_) =~ /(.). \1. \1. (.). \2.|(.). \1. (.). \2. \2./;
	return "G".substr($hand[2],0,1);
}

sub Three {
	return "" unless join(' ', @_) =~ /(.). \1. \1./;
	return "D$1";
}

sub TwoPair {
	return "" unless join(' ', @_) =~ /(.). \1. (.). \2./;
	return "C$2$1";
}

sub Pair {
	my $hand = join ' ', @_;
	return "" unless $hand =~ /(.). \1./;
	my $pair = $1;
	$hand =~ /(.). (.). (.). (.). (.)./;
	return "B$pair$5$4$3$2$1";
}

sub High {
	join(' ', @_) =~ /(.). (.). (.). (.). (.)./;
	return "A$5$4$3$2$1";
}

sub Value {
	my @hand = @_;
	#print "\nValue hand=@hand.\n";
	my @games;
	push @games, &High(@hand);
	push @games, &Pair(@hand);
	push @games, &TwoPair(@hand);
	push @games, &Three(@hand);
	push @games, &Straight(@hand);
	push @games, &Flush(@hand);
	push @games, &FullHouse(@hand);
	push @games, &Four(@hand);
	push @games, &StraightFlush(@hand);
	@games = sort @games;
	#print "Value returns ".join('/',@games)."\n";
	return $games[-1];
}

our %handNames = (
	A => "high card", 
	B => "one pair", 
	C => "two pair", 
	D => "3 of a kind", 
	E => "straight", 
	F => "flush",
	G => "full house",
	H => "4 of a kind",
	I => "straight flush" );
our (@hands);
our $ct = 0;
print "\nTesting\n";
print "\nstraight flush:", &Value(qw(JH KH LH MH NH));
print "\nfour:", &Value(qw(JH JS JC JD NH));
print "\nfull hand:", &Value(qw(JH JS JC KD KH));
print "\nflush:", &Value(qw(BH KH LH MH NH));
print "\nstraight:", &Value(qw(JH KS LH MH NH));
print "\nthree:", &Value(qw(JH JS JC KD LH));
print "\n2 pair:", &Value(qw(JH JS KC KD LH));
print "\n1 pair:", &Value(qw(IH JS KC KD LH));
print "\nhigh card:", &Value(qw(GH HS IC KD LH));
print "\nEnd test\n";
#exit;
while (<>) {
	chop; chop;
	my $original = $_;
	#print "o = $original\n";
	my $converted = $_;
	$converted =~ tr/23456789TJQKA/BCDEFGHIJKLMN/;
	next unless $converted =~ /^(.. .. .. .. ..) (.. .. .. .. ..)/;
	$hands[0] = [sort split / /, $1];
	$hands[1] = [sort split / /, $2];
	#print "o2 = $original\n";
	my $h1 = &Value(@{$hands[0]});
	my $h1n = $handNames{substr($h1,0,1)};
	my $h2 = &Value(@{$hands[1]});
	my $h2n = $handNames{substr($h2,0,1)};
	print "$original -> $h1n vs $h2n - ";
	if ($h1 gt $h2)
	{
		$ct = $ct + 1;
		print "$ct wins!\n";
	}
	else
	{
		print "loses\n";
	}
	#print "\n\n";
}
print "Finished with $ct";