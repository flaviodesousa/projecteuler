#! /usr/bin/perl -W

sub crack {
	my @key = @_;
	my $off = 0;
	my @message;
	for my $cipher (@cipher) {
		my $msg = chr($cipher ^ $key[$off++ % 3]);
		return if $msg !~ /^[a-zA-Z0-9 (),:;.'"!]+$/;
		push @message, $msg;
	}
	print "key=[";
	for my $ch (@key) {
		print chr($ch);
	}
	print "]\n";
	my $sum = 0;
	for my $ch (@message) {
		print $ch;
		$sum += ord($ch);
	}
	printf "\nsum=%d\n\n", $sum;
}

sub combineLetters {
	my @existing = @_;
	if (scalar(@existing) == 3) {
		&crack(@existing);
	} else {
		for my $ch (ord('a')..ord('z')) {
			&combineLetters(@existing, $ch);
		}
	}
}

our (@cipher);
open CIPHER, "cipher1.txt"
	or die "$!";
while (<CIPHER>) {
	chomp;
	push @cipher, split /,/;
}
close CIPHER;

&combineLetters(());
