#pragma once

#define BOARD_UNKNOWN -1

#define BOARD_WEEDO_62A			800
#define BOARD_WEEDO_62B			801		
#define BOARD_WEEDO_62V1		802	
#define BOARD_WEEDO_61A			803	
#define BOARD_WEEDO_61B			804	
#define BOARD_WEEDO_71A			805	
#define BOARD_WEEDO_72A			806	
#define BOARD_WEEDO_73A			807
#define BOARD_WEEDO_73B			808
#define BOARD_WEEDO_74A			809
#define BOARD_WEEDO_83A         810
#define BOARD_WEEDO_83B         811


#define MB(board) (defined(BOARD_##board) && MOTHERBOARD==BOARD_##board)
