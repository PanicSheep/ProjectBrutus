File formats
------------

ps-file:
	(uint64) Players bitboard
	(uint64) Opponents bitboard
	(int8)   Depth to which the position is solved (127 means solved exact)
	(uint8)  Selectivity under which the position was solved (0 means 100%)
	(int8)   Score for mutual perfect play


psp-file:
	(uint64) Players bitboard
	(uint64) Opponents bitboard
	(int8)   Depth to which the position is solves (127 means solved exact)
	(uint8)  Selectivity under which the position was solved (0 means 100%)
	(int8)   Score for mutual perfect play
	(uint8)  PV-Line (64 means no move)
	(uint8)  PV-Line (64 means no move)
	(uint8)  PV-Line (64 means no move)
	(uint8)  PV-Line (64 means no move)
	(uint8)  PV-Line (64 means no move)