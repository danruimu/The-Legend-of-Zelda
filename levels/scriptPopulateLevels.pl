#!/usr/bin/perl

my @numbers = (1, 3, 4, 5, 6, 7, 8);
my @chars = ('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P');
my $file_ext = ".txt";

my @enemies = ("octorok-b", "fat_dog-o");
my $max_enemies = 10;

my $scene_width = 16;
my $scene_height = 11;

foreach $n (@numbers) {
	foreach $c (@chars) {
		my $file = $c.$n.$file_ext;
		my $how_many = int(rand($max_enemies)) + 1;
		open(MYFILE, '>>'.$file);
		print MYFILE "\n";
		print MYFILE $how_many; print MYFILE "\n";
		my @xpositions;
		my @ypositions;
		while($how_many > 0) {
			my $next_enemy = int(rand(2));
			my $how_many_next = int(rand($how_many)) + 1;
			print MYFILE $enemies[$next_enemy]." ".$how_many_next;
			$how_many = $how_many - $how_many_next;
			my $map = `head -n 11 $file`;
			my @map_lines = split(/^/, $map);
			while($how_many_next > 0) {
				my $x = int(rand($scene_width)) + 1;
				my $y = int(rand($scene_height)) + 1;
				# CHECKING POSITION
				my $line = $map_lines[$scene_height-$y];
				@xLine = split(/,/, $line);
				$tile = $xLine[$x-1];
				if($tile == "3" || $tile == "9" || $tile == "15" || $tile == "59" || $tile == "65" || $tile == "71" || ($tile >= "76" && $tile <= "78") || ($tile >= "82" && $tile <= "84") || ($tile >= "88" && $tile <="89") || ($tile >= "94" && $tile <= "96") || ($tile >= "100" && $tile <= "102") || ($tile >= "106" && $tile <="108") || ($tile >= "112" && $tile <= "114") || ($tile >= "118" && $tile <= "120") || ($tile >= "124" && $tile <="126")) {
					
					####################################
					# WE MUST CHECK IF AT THIS POSITION
					# THERE IS ANOTHER ENEMY
					####################################
					my $b = 0;
					for($i = 0; i < $xpositions+1 && $b == 0; $i++) {
						if($x != @xpositions[$i] && $y != @ypositions[$i]) {
							$b = 1;
						}
					}
					if($b == 1) {
						print MYFILE " ".$x." ".$y;
						$xpositions[$xpositions+1] = $x;
						$ypositions[$ypositions+1] = $y;
						$how_many_next--;
					}
				}
				# if the pos is not valid, repeat this iteration and do
				# not print anything
			}
			print MYFILE "\n";
		}
		close(MYFILE);
	}
}
