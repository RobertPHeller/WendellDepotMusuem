variable opticalSensors -array {
	West_Stage_Exit_3 {
		{West Stage Exit 3}
		05.01.01.01.22.83.00.04
		05.01.01.01.22.83.00.05
	}
	West_Stage_Exit_1 {
		{West Stage Exit 1}
		05.01.01.01.22.83.00.06
		05.01.01.01.22.83.00.07
	}
	West_Stage_Exit_2 {
		{West Stage Exit 2}
		05.01.01.01.22.83.00.10
		05.01.01.01.22.83.00.11
	}
	West_Stage_Exit_4 {
		{West Stage Exit 4}
		05.01.01.01.22.83.00.12
		05.01.01.01.22.83.00.13
	}
	East_Double_Track_Entrance {
		{East Double Track Entrance}
		05.01.01.01.22.82.00.00
		05.01.01.01.22.82.00.01
	}
	East_Double_Track_Frog_Stop {
		{East Double Track Frog Stop}
		05.01.01.01.22.82.00.04
		05.01.01.01.22.82.00.05
	}
	East_Double_Track_Straight_Stop {
		{East Double Track Straight Stop}
		05.01.01.01.22.82.00.02
		05.01.01.01.22.82.00.03
	}
	West_Double_Track_Entrance {
		{West Double Track Entrance}
		05.01.01.01.22.81.00.00
		05.01.01.01.22.81.00.01
	}
	West_Double_Track_Frog_Stop {
		{West Double Track Frog Stop}
		05.01.01.01.22.81.00.04
		05.01.01.01.22.81.00.05
	}
	West_Double_Track_Straight_Stop {
		{West Double Track Straight Stop}
		05.01.01.01.22.81.00.02
		05.01.01.01.22.81.00.03
	}
	Stage_West_1_3_Entrance {
		{Stage West 1-3 Entrance}
		05.01.01.01.22.83.00.00
		05.01.01.01.22.83.00.01
	}
	Stage_West_2_4_Entrance {
		{Stage West 2-4 Entrance}
		05.01.01.01.22.83.00.02
		05.01.01.01.22.83.00.03
	}
	Stage_East_1_3_Entrance {
		{Stage East 1-3 Entrance}
		05.01.01.01.22.84.00.04
		05.01.01.01.22.84.00.05
	}
	Stage_East_2_4_Entrance {
		{Stage East 2-4 Entrance}
		05.01.01.01.22.84.00.06
		05.01.01.01.22.84.00.07
	}
	Wendell_Depot_Rd_Grade_Crossing {
		{Wendell Depot Rd Grade Crossing}
		05.01.01.01.22.80.00.00
		05.01.01.01.22.80.00.01
	}
	East_Stage_Exit_3 {
		{East Stage Exit 3}
		05.01.01.01.22.83.00.14
		05.01.01.01.22.83.00.15
	}
	East_Stage_Exit_1 {
		{East Stage Exit 1}
		05.01.01.01.22.83.00.16
		05.01.01.01.22.83.00.17
	}
	East_Stage_Exit_2 {
		{East Stage Exit 2}
		05.01.01.01.22.84.00.00
		05.01.01.01.22.84.00.01
	}
	East_Stage_Exit_4 {
		{East Stage Exit 4}
		05.01.01.01.22.84.00.02
		05.01.01.01.22.84.00.03
	}
}
variable turnouts -array {
	Stage_East_1_3 {
		{Stage East 1-3}
		{
			05.01.01.01.22.84.00.18
			05.01.01.01.22.84.00.19
		} {
			05.01.01.01.22.84.00.30
			05.01.01.01.22.84.00.31
		}
	}
	Stage_East_2_4 {
		{Stage East 2-4}
		{
			05.01.01.01.22.84.00.1A
			05.01.01.01.22.84.00.1B
		} {
			05.01.01.01.22.84.00.32
			05.01.01.01.22.84.00.33
		}
	}
	Stage_West_2_4 {
		{Stage West 2-4}
		{
			05.01.01.01.22.83.00.32
			05.01.01.01.22.83.00.33
		} {
			05.01.01.01.22.83.00.50
			05.01.01.01.22.83.00.51
		}
	}
	Stage_West_1_3 {
		{Stage West 1-3}
		{
			05.01.01.01.22.83.00.30
			05.01.01.01.22.83.00.31
		} {
			05.01.01.01.22.83.00.50
			05.01.01.01.22.83.00.51
		}
	}
	East_Double_Track {
		{East Double Track}
		{
			05.01.01.01.22.82.00.18
			05.01.01.01.22.82.00.19
		} {
			05.01.01.01.22.82.00.30
			05.01.01.01.22.82.00.31
		}
	}
	West_Double_Track {
		{West Double Track}
		{
			05.01.01.01.22.81.00.18
			05.01.01.01.22.81.00.19
		} {
			05.01.01.01.22.81.00.30
			05.01.01.01.22.81.00.31
		}
	}
}
variable signals -array {
	WestEN {
		{WestEN}
		{
			Stop
			{{Stop}
			05.01.01.01.22.81.00.40}
			Slow_Aproach
			{{Slow Aproach}
			05.01.01.01.22.81.00.41}
			Slow_Clear
			{{Slow Clear}
			05.01.01.01.22.81.00.42}
			Aproach
			{{Aproach}
			05.01.01.01.22.81.00.43}
			Clear
			{{Clear}
			05.01.01.01.22.81.00.44}
		}
	}
	WestF1 {
		{WestF1}
		{
			Stop
			{{Stop}
			05.01.01.01.22.81.00.60}
			Aproach
			{{Aproach}
			05.01.01.01.22.81.00.61}
			Slow_Clear
			{{Slow Clear}
			05.01.01.01.22.81.00.62}
		}
	}
	WestFM {
		{WestFM}
		{
			Stop
			{{Stop}
			05.01.01.01.22.81.00.50}
			Aproach
			{{Aproach}
			05.01.01.01.22.81.00.51}
			Clear
			{{Clear}
			05.01.01.01.22.81.00.52}
		}
	}
	WestYardEntry1_3 {
		{WestYardEntry1-3}
		{
			Stop
			{{Stop}
			05.01.01.01.22.83.00.60}
			Slow_Aproach
			{{Slow Aproach}
			05.01.01.01.22.83.00.61}
			Slow_Clear
			{{Slow Clear}
			05.01.01.01.22.83.00.62}
			Aproach
			{{Aproach}
			05.01.01.01.22.83.00.63}
			Clear
			{{Clear}
			05.01.01.01.22.83.00.64}
		}
	}
	WestYardEntry2_4 {
		{WestYardEntry2-4}
		{
			Stop
			{{Stop}
			05.01.01.01.22.83.00.70}
			Slow_Aproach
			{{Slow Aproach}
			05.01.01.01.22.83.00.71}
			Slow_Clear
			{{Slow Clear}
			05.01.01.01.22.83.00.72}
			Aproach
			{{Aproach}
			05.01.01.01.22.83.00.73}
			Clear
			{{Clear}
			05.01.01.01.22.83.00.74}
		}
	}
	EastYardEntry1_3 {
		{EastYardEntry1-3}
		{
			Stop
			{{Stop}
			05.01.01.01.22.83.00.D0}
			Slow_Aproach
			{{Slow Aproach}
			05.01.01.01.22.83.00.D1}
			Slow_Clear
			{{Slow Clear}
			05.01.01.01.22.83.00.D2}
			Aproach
			{{Aproach}
			05.01.01.01.22.83.00.D3}
			Clear
			{{Clear}
			05.01.01.01.22.83.00.D4}
		}
	}
	EastYardEntry2_4 {
		{EastYardEntry2-4}
		{
			Stop
			{{Stop}
			05.01.01.01.22.83.00.E0}
			Slow_Aproach
			{{Slow Aproach}
			05.01.01.01.22.83.00.E1}
			Slow_Clear
			{{Slow Clear}
			05.01.01.01.22.83.00.E2}
			Aproach
			{{Aproach}
			05.01.01.01.22.83.00.E3}
			Clear
			{{Clear}
			05.01.01.01.22.83.00.E4}
		}
	}
	EastExit4__Dwarf_ {
		{EastExit4 (Dwarf)}
		{
			Hold
			{{Hold}
			05.01.01.01.22.83.00.F0}
			Go
			{{Go}
			05.01.01.01.22.83.00.F1}
		}
	}
	EastExit2__Dwarf_ {
		{EastExit2 (Dwarf)}
		{
			Hold
			{{Hold}
			05.01.01.01.22.83.00.F4}
			Go
			{{Go}
			05.01.01.01.22.83.00.F5}
		}
	}
	EastExit1__Dwarf_ {
		{EastExit1 (Dwarf)}
		{
			Hold
			{{Hold}
			05.01.01.01.22.83.00.F8}
			Go
			{{Go}
			05.01.01.01.22.83.00.F9}
		}
	}
	EastExit3__Dwarf_ {
		{EastExit3 (Dwarf)}
		{
			Hold
			{{Hold}
			05.01.01.01.22.83.00.FC}
			Go
			{{Go}
			05.01.01.01.22.83.00.FD}
		}
	}
	WestExit3__Dwarf_ {
		{WestExit3 (Dwarf)}
		{
			Hold
			{{Hold}
			05.01.01.01.22.83.00.80}
			Go
			{{Go}
			05.01.01.01.22.83.00.81}
		}
	}
	WestExit1__Dwarf_ {
		{WestExit1 (Dwarf)}
		{
			Hold
			{{Hold}
			05.01.01.01.22.83.00.84}
			Go
			{{Go}
			05.01.01.01.22.83.00.85}
		}
	}
	WestExit2__Dwarf_ {
		{WestExit2 (Dwarf)}
		{
			Hold
			{{Hold}
			05.01.01.01.22.83.00.88}
			Go
			{{Go}
			05.01.01.01.22.83.00.89}
		}
	}
	WestExit4__Dwarf_ {
		{WestExit4 (Dwarf)}
		{
			Hold
			{{Hold}
			05.01.01.01.22.83.00.C0}
			Go
			{{Go}
			05.01.01.01.22.83.00.C1}
		}
	}
	EastEN {
		{EastEN}
		{
			Stop
			{{Stop}
			05.01.01.01.22.82.00.40}
			Slow_Aproach
			{{Slow Aproach}
			05.01.01.01.22.82.00.41}
			Slow_Clear
			{{Slow Clear}
			05.01.01.01.22.82.00.42}
			Aproach
			{{Aproach}
			05.01.01.01.22.82.00.43}
			Clear
			{{Clear}
			05.01.01.01.22.82.00.44}
		}
	}
	EastF1 {
		{EastF1}
		{
			Stop
			{{Stop}
			05.01.01.01.22.82.00.50}
			Slow_Aproach
			{{Slow Aproach}
			05.01.01.01.22.82.00.51}
			Slow_Clear
			{{Slow Clear}
			05.01.01.01.22.82.00.52}
		}
	}
	EastFM {
		{EastFM}
		{
			Stop
			{{Stop}
			05.01.01.01.22.82.00.60}
			Aproach
			{{Aproach}
			05.01.01.01.22.82.00.61}
			Clear
			{{Clear}
			05.01.01.01.22.82.00.62}
		}
	}
}
