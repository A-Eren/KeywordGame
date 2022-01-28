#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include<Windows.h>
struct Game_data {
	int score_background;  //next updated scora value
	int timer_change;  // to determine did timer change or not
	int timer;		// timer itself
	int timer_storage; // to store timer while changing timer to out from a loop
	int menu_flag; // to determine the which button I am using while on menu
	char length[11]; // for checking to end of 10 leght strings
	char string[8][10][11]; //keeping string's in 3 dimension array. First dimension presents level, second one presents queue of the string, the last one is for strings and last null character
	int word_counter[10]; // each element of array presents number of word at the level. For exp. word[3]=number of word that can appear at level 3. So I can randomize at this range to run from errors
	int lvl; // integer value of level
	int sub_lvl;// while taking words from 3 dimension array I am using this as a helper.It depends on lvl
	int score;// score
	int word;// = rand() % word_counter[lvl]; int value of the word on line
	int saved_bombs;// to calculate leveling up
	int mode;// switch between game/menu/pause/end_game 
	int k;// using this guy as carry while taking words from file 
	int length_passer;// comparing length and it's previous value. Also this one helping to taking words from file
	int enter_flag;	// to check did enter pushed or not
	int nt_letters;// nt_letter for number of typed letters
	int plane_locationx; // location of plane at x horizon
	int bomb_locationx; // loaction of bomb at x horizon
	int bomb_locationy; //loaction of bomb at y horizon
	int sky_distance; //height of plane
	int bombdrop; // checking the case of bomb droped 
	int bombdrop_change; // checking the case bomdrop changes
	int bombed_time;  // value of time variable when bomb drops
	char save_file_name[20]; //name of save file
	char load_file_name[20]; // name of loading file
	FILE* fptr_save,*fptr_load; // file pointers for save & load
};
int main()
{	
	srand(time(NULL));                         
	struct Game_data game,save,none;
	game.menu_flag = 0;
	game.lvl = 0;
	game.sub_lvl = game.lvl;
	game.score = 0;
	game.score_background = 0;
	game.saved_bombs = 0;
	game.k = 0;
	game.enter_flag = 0;
	game.nt_letters = 0;
	game.sky_distance = 15;
	game.bombed_time = 0;
	game.bomb_locationy = 0;
	game.bombdrop = 0;
	game.bombdrop_change = 1;
	FILE* fptr_wordlist, * fptr_length;
	fptr_wordlist = fopen("codewords.txt", "r");
	fptr_length = fopen("codewords.txt", "r");
	if (fptr_wordlist == NULL)
	{
		printf("Error!");
		exit(1);
	}
	if (fptr_length == NULL)
	{
		printf("Error!");
		exit(1);
	}
	game.length_passer = 3;
	for (int i = 0; i < 100; i++) {
		fscanf(fptr_length, "%s", game.length);
		int length_value = strlen(game.length);
		if (length_value == 1) {
			game.word_counter[game.length_passer - 3] = game.k;
			break;
		}
		else if (length_value != game.length_passer) {
			game.word_counter[game.length_passer - 3] = game.k;
			game.k = 0;
			game.length_passer = length_value;
		}
		fscanf(fptr_wordlist, "%s", game.string[length_value - 3][game.k]);
		game.k++;
	} // this command takes word's from file
	none = game;     // creating none structure for reseting and starting new game
	game.word = rand() % game.word_counter[game.lvl];   // generates random word for first level
	game.mode = 1;   //sets starting menu                                                                 
	// till there every command setups for game
	while(1){
	while (game.mode == 0) {  // opens determined menu or game 
		gameplay(&game,&none);
	}
	while (game.mode == 1) {
		start_menu(&game,&save,&none); 
	}
	while (game.mode == 2) {
		pause_menu(&game,&save,&none);
	}
	while (game.mode == 3) { // this is end game it just wait input to process
		while (_kbhit()) {
			char c = _getch();
			if (c == 121){
				system("cls");
				game.mode = 0;
			}
			else if (c==110){
				system("cls");
				game.mode = 1;
			}
		}
	}
	}
	return 0;
}
int gameplay(struct Game_data *game, struct Game_data* none) {  //game itself
	while ((*game).mode == 0) {
	for ((*game).timer = 0; (*game).timer < 20; (*game).timer++) {			//starting a timer for calculating relative passing time at game
		if((*game).timer_change==1){
			(*game).timer = (*game).timer_storage;		//if time changed this function sets time's value at storage 
			(*game).timer_change = 0;
		}
		if ((*game).bombdrop_change==1) {				//if bomb dropped this function makes preparations for bomb
			(*game).bomb_locationy = 0;
			(*game).bomb_locationx = (*game).plane_locationx;
			(*game).bombed_time = (*game).timer;
			(*game).bombdrop_change = 0;
			(*game).bombdrop = 1;
		}
		if ((*game).bomb_locationy == (*game).sky_distance) {	//if bomb down to city this function end's gameplay
			system("cls"); //explosion ascii art at next line
			printf("                               ________________\n                          ____/ (  (    )   )  \\___\n                         /( (  (  )   _    ))  )   )\\ \n                       ((     (   )(    )  )   (   )  )\n                     ((/  ( _(   )   (   _) ) (  () )  )\n                    ( (  ( (_)   ((    (   )  .((_ ) .  )_\n                   ( (  )    (      (  )    )   ) . ) (   )\n                  (  (   (  (   ) (  _  ( _) ).  ) . ) ) ( )\n                  ( (  (   ) (  )   (  ))     ) _)(   )  )  )\n                 ( (  ( \\ ) (    (_  ( ) ( )  )   ) )  )) ( )\n                  (  (   (  (   (_ ( ) ( _    )  ) (  )  )   )\n                 ( (  ( (  (  )     (_  )  ) )  _)   ) _( ( )\n                  ((  (   )(    (     _    )   _) _(_ (  (_ )\n                   (_((__(_(__(( ( ( |  ) ) ) )_))__))_)___)\n                   ((__)        \\\\||lll|l||///          \\_))\n                            (   /(/ (  )  ) )\\   )\n                          (    ( ( ( | | ) ) )\\   )\n                           (   /(| / ( )) ) ) )) )\n                         (     ( ((((_(|)_)))))     )\n                          (      ||\\(|(|)|/||     )\n                        (        |(||(||)||||        )\n                          (     //|/l|||)|\\ \\     )\n                        (/ / //  /|//||||\\ \\ \\ \\ _)\n-------------------------------------------------------------------------------\n");  //kaboooom
			for(int i = 0; i <= 10; i++)
				printf("   ");										//prints string at middle
			printf("You Lose\n",(*game).score);
			for (int i = 0; i <= 14; i++)
				printf("  ");
			printf("Your score is %d\n",(*game).score);
			for (int i = 0; i <= 11; i++)
				printf(" ");
			printf("Press y to try again or press n to escape main menu!!");
			(*game) = (*none);				//resets game to new start
			(*game).mode = 3;
			(*game).word = rand()%(*game).word_counter[(*game).lvl];
			break;
		}
		draw_plane(game);  //draws plane
		(*game).plane_locationx = (*game).timer;  //sets plane location 
		if ((*game).bombdrop == 1) {
			(*game).bomb_locationy = ((*game).timer - (*game).bombed_time + 20) % 20;  //a little hetch for block bom_loaction became negative
			draw_bomb(game);			//draws bomb
		}
		else if ((*game).bombdrop == 0)		//if there is not a bomb just draws sky
			printf("\n\n");
		draw_city(game);  //draws city
		delay(450-((*game).lvl*50),game);  //changes game speed relevant to level
		system("cls");  //to animating drawings
		}
	}
}
int pause_menu(struct Game_data *game, struct Game_data* save, struct Game_data* none) { //generates pause menu 
	if ((*game).menu_flag == 0)
		new_game(game,none);
	else if ((*game).menu_flag == 1)
		load_game(game,save);
	else if ((*game).menu_flag == 2)
		save_game(game);
	else if ((*game).menu_flag == 3)
		return_game(game);
	else if ((*game).menu_flag == 4)
		quit_game(game);
	
}
int start_menu(struct Game_data *game, struct Game_data* save, struct Game_data* none){//generates starting menu

	if ((*game).menu_flag == 0)
		new_game(game,none);
	else if ((*game).menu_flag == 1)
		load_game(game,save);
	else if ((*game).menu_flag == 2)
		save_game(game);
	else if ((*game).menu_flag == 3)
		quit_game(game);
	else if ((*game).menu_flag == 4)
		(*game).menu_flag = 0;
	
}
int delay(int number_of_milli_seconds,struct Game_data *game) //The code mostly delays because of processing speed. So I wrote all catching codes to delay. 
{
	clock_t start_time = clock();
	while (clock() < start_time + number_of_milli_seconds); { //starting dely
		while (_kbhit()) { // also waits for my keyboard input
			char c = _getch();
			if((*game).mode !=0){ //pocesses on menu
				if (c == 13) //tracks enter key 
					(*game).enter_flag++;

				else if (c == -32){ //tracks drection keys begining
				c = _getch();
			if (c == 80){ //tracks key down
				if ((*game).mode == 1 && (*game).menu_flag == 3)
					(*game).menu_flag = (*game).menu_flag + 2;
				else
					(*game).menu_flag++;
			}
			else if (c == 72)//tracks key up
				if ((*game).mode == 1 && (*game).menu_flag == 0)
					(*game).menu_flag = (*game).menu_flag - 2;
			else
				(*game).menu_flag--;
			(*game).menu_flag = ((*game).menu_flag + 5) % 5; // menu_flag shouldn't be -1 because it's crashes at if statements , I can also add negative values of numbers
		}                                      // to if statements but I found this solution smarter and better.
		}
			else if ((*game).mode == 0) {  //gameplay
				if (c == (*game).string[(*game).lvl][(*game).word][(*game).nt_letters]) {//checks word's characters
					(*game).score_background++; //keeps scores changes
					(*game).nt_letters++;
					if ((*game).nt_letters == (*game).lvl + 3) { //if a word completed makes some changes
						(*game).bombdrop_change = 1;
						(*game).nt_letters = 0;
						(*game).saved_bombs++;
						(*game).score = (*game).score_background;
						(*game).word = rand() % (*game).word_counter[(*game).lvl];
					}
					if ((*game).saved_bombs == 5 && (*game).lvl !=7) { //if player defused 5 bomb this function levels up
						(*game).lvl++;
						(*game).word = rand() % (*game).word_counter[(*game).lvl];
						(*game).saved_bombs = 0;
					}

				}
			else if (c == 27) { // esc=27 pause
					(*game).timer_change=1;
					 (*game).mode = 2;
					 (*game).timer_storage = (*game).timer;
					 (*game).timer = 21;
				 }
			else //for mistyped characters
				 (*game).score_background--;
			}		
		}
	}
}
int new_game(struct Game_data *game, struct Game_data*none) {
	if ((*game).enter_flag==1){	//if enter tabbed resets game
		(*none).mode = 0;
		*game = *none;
		(*game).word = rand() % (*game).word_counter[(*game).lvl];
	}
	else{ //just some animation
	printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
	printf("\n\n _   _                       _____ \n| \\ | |                     |  __ \\ \n|  \\| | _____      __       | |  \\/ __ _ _ __ ___   ___   \n| .   |/ _ \\ \\ /\\ / /       | | __ / _  |  _   _ \\ / _ \\  \n| |\\  |  __/\\ V  V /        | |_\\ \\ (_| | | | | | |  __/  \n\\_| \\_/\\___| \\_/\\_/          \\____/\\__,_|_| |_| |_|\\___|");
	printf("\n\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
	delay(400,game);
	system("cls");
	printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
	printf("\n\n\n\n\n\n\n");
	printf("\n\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
	delay(300,game);
	system("cls");
	}
}
int load_game(struct Game_data *game, struct Game_data* save) {
	if ((*game).enter_flag == 1) { // if enter tabbed loads mentioned game's save
		FILE* ptr;
		system("cls");
		printf("Name of the file you want to load:\n");
		scanf("%s", &(*game).load_file_name);
		char* extension = ".txt";
		char fileSpec[20]; 
		snprintf(fileSpec, sizeof(fileSpec), "%s%s", &(*game).load_file_name, extension); // combines two varaible to use at fopen
		ptr = fopen(fileSpec, "rb");
		if (ptr == NULL)
		{
			printf("\nThere is not a such a directory or file.\n");
			delay(700,game);
		}
		else{
		while(fread(save, sizeof(struct Game_data), 1, ptr)){ //loading files to save pointer  it says save pointer but it's actualy does both of save and load
			*game = *save; //loads all 
		}
		fclose(ptr); //closes file
		}
		(*game).enter_flag = 0; //going to menu
		system("cls");
		(*game).mode = 0;
	}
	else { //some animations
		printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
		printf("\n\n _                     _     _____ \n| |                   | |   |  __ \\\n| |     ___   __ _  __| |   | |  \\/ __ _ _ __ ___   ___  \n| |    / _ \\ / _  |/ _  |   | | __ / _  |  _   _ \\ / _ \\ \n| |___| (_) | (_| | (_| |   | |_\\ \\ (_| | | | | | |  __/  \n\\_____/\\___/ \\__,_|\\__,_|    \\____/\\__,_|_| |_| |_|\\___| \n");
		printf("\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
		delay(400, game);
		system("cls");
		printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
		printf("\n\n\n\n\n\n\n\n");
		printf("\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
		delay(300, game);
		system("cls");
	}
}
int save_game(struct Game_data *game) { //saves game
	if ((*game).enter_flag == 1){
		system("cls");
		printf("Name the save file:\n");
		scanf("%s", &(*game).save_file_name);
		char* extension = ".txt";
		char fileSpec[20];  //does same thing with load function
		snprintf(fileSpec, sizeof(fileSpec), "%s%s", &(*game).save_file_name, extension);
		(*game).fptr_save = fopen(fileSpec, "wb");//creates file 
		if ((*game).fptr_save == NULL)
			printf("Error");
		fwrite(game, sizeof(struct Game_data), 1, (*game).fptr_save); //writes current varaibles
		fclose((*game).fptr_save);//closes file
		(*game).enter_flag = 0; //goes to menu
	}
	else {//some animations
	printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
	printf("\n\n _____                       _____ \n/  ___|                     |  __ \\ \n\\  --.  __ ___   _____      | |  \\/ __ _ _ __ ___   ___   \n  --. \\/ _  \\ \\ / / _ \\     | | __ / _  |  _   _ \\ / _ \\ \n/\\__/ / (_| |\\ V /  __/     | |_\\ \\ (_| | | | | | |  __/  \n\\____/ \\__,_| \\_/ \\___|     \\_____/\\__,_|_| |_| |_|\\___| \n");
	printf("\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
	delay(400,game);
	system("cls");
	printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
	printf("\n\n\n\n\n\n\n\n");
	printf("\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
	delay(300,game);
	system("cls");
	}
}
int return_game(struct Game_data *game) {
	if ((*game).enter_flag == 1){
		(*game).timer_change=1; //returns game and continues
		(*game).enter_flag = 0;
		(*game).mode = 0;
	}
	else{ //some animations
	printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
	printf("\n\n______     _                      _____ \n| ___ \\   | |                    |  __ \\\n| |_/ /___| |_ _   _ _ __ _ __   | |  \\/ __ _ _ __ ___   ___ \n|    // _ \\ __| | | | '__| '_ \\  | | __ / _` | '_ ` _ \\ / _ \\\n| |\\ \\  __/ |_| |_| | |  | | | | | |_\\ \\ (_| | | | | | |  __/\n\\_| \\_\\___|\\__|\\__,_|_|  |_| |_|  \\____/\\__,_|_| |_| |_|\\___|");
	printf("\n\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
	delay(400,game);
	system("cls");
	printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
	printf("\n\n\n\n\n\n\n");
	printf("\n\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
	delay(300,game);
	system("cls");
	}
}
int quit_game(struct Game_data *game) {
	if ((*game).enter_flag==1) //exits game
		exit(1);
	//otherwise we got got some animations again
	printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
	printf("\n\n  _____       _ _             _____ \n|  _  |     (_) |           |  __ \\  \n| | | |_   _ _| |_          | |  \\/ __ _ _ __ ___   ___   \n| | | | | | | | __|         | | __ / _  |  _   _ \\ / _ \\  \n\\ \\_/ / |_| | | |_          | |_\\ \\ (_| | | | | | |  __/  \n \\_/\\_\\\\__,_|_|\\__|          \\____/\\__,_|_| |_| |_|\\___|");
	printf("\n\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
	delay(400,game);
	system("cls");
	printf("		   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\");
	printf("\n\n\n\n\n\n\n");
	printf("\n\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/");
	delay(300,game);
	system("cls");
}
int draw_plane(struct Game_data* game) { //draws plane depending on location and timer
	int k=0;
	for (int space_spammer = 0; space_spammer <= (*game).plane_locationx; space_spammer++){
		printf("   ");
		k++;
	}
	printf("(|__.-. |");
	for(int space_spammer=k;space_spammer <=20;space_spammer++) // I got so many error there from plane's location that was the only solution I could find.
		printf("   ");
	printf("Score=%d\n", (*game).score);
	for (int space_spammer = 0; space_spammer <= (*game).plane_locationx; space_spammer++)
		printf("   ");
	printf("===== _]+\n");
	for (int space_spammer = 0; space_spammer <= (*game).plane_locationx; space_spammer++)
		printf("   ");
	printf("        |\n");
}
int draw_bomb(struct Game_data* game) { //draws bomb depending on location and timer
	for (int row_spammer = 0; row_spammer < (*game).bomb_locationy; row_spammer++)
		printf("\n");
	for (int space_spammer = 0; space_spammer < (*game).bomb_locationx; space_spammer++)
		printf("   ");
	printf("<|>\n");
	for (int space_spammer = 0; space_spammer < (*game).bomb_locationx; space_spammer++)
		printf("   ");
	printf(" |_");
	for (int t = (*game).nt_letters; t <= (*game).lvl + 3; t++) 
		printf("%c", (*game).string[(*game).lvl][(*game).word][t]);
	printf("\n");
	for (int space_spammer = 0; space_spammer < (*game).bomb_locationx; space_spammer++)
		printf("   ");
	printf(" *");
}
int draw_city(struct Game_data* game) { //draws city depending on location and timer
	for (int row_spammer = (*game).bomb_locationy; row_spammer <(*game).sky_distance ; row_spammer++)
		printf("\n");
	printf("\n _____        _____    _________________ ________    _________\n|o o o|_______|    |__|               | | # # # |____|o o o o|\n|o o o|* * *|: ::|. . |               |o| # # # |. . |o o o o|\n|o o o|* * *|::  |. . | []  []  []  []|o| # # # |. . |o o o o|\n|o o o|**** |:  :| . .| []  []  []    |o| # # # |. . |o o o o|\n|_[]__|__[]_|_||_|__< |____________;;_|_|___[]__|_.|_|__[]___|");
}
//Up Arrow
//				   ____ \n                  / /\\ \\ \n                 / /  \\ \\ \n                / /    \\ \\\n               / /      \\ \\\n              /_/        \\_\\
//New game
//\n\n _   _                       _____ \n| \\ | |                     |  __ \\ \n|  \\| | _____      __       | |  \\/ __ _ _ __ ___   ___   \n| .   |/ _ \\ \\ /\\ / /       | | __ / _  |  _   _ \\ / _ \\  \n| |\\  |  __/\\ V  V /        | |_\\ \\ (_| | | | | | |  __/  \n\\_| \\_/\\___| \\_/\\_/          \\____/\\__,_|_| |_| |_|\\___|
//
//Load game
//\n\n _                     _     _____ \n| |                   | |   |  __ \\\n| |     ___   __ _  __| |   | |  \\/ __ _ _ __ ___   ___  \n| |    / _ \\ / _  |/ _  |   | | __ / _  |  _   _ \\ / _ \\ \n| |___| (_) | (_| | (_| |   | |_\\ \\ (_| | | | | | |  __/  \n\\_____/\\___/ \\__,_|\\__,_|    \\____/\\__,_|_| |_| |_|\\___| \n
//
//Save game
//\n\n _____                       _____ \n/  ___|                     |  __ \\ \n\\  --.  __ ___   _____      | |  \\/ __ _ _ __ ___   ___   \n  --. \\/ _  \\ \\ / / _ \\     | | __ / _  |  _   _ \\ / _ \\ \n/\\__/ / (_| |\\ V /  __/     | |_\\ \\ (_| | | | | | |  __/  \n\\____/ \\__,_| \\_/ \\___|     \\_____/\\__,_|_| |_| |_|\\___| \n
//
// Return game
// ______     _                      _____ \n| ___ \\   | |                    |  __ \\\n| |_/ /___| |_ _   _ _ __ _ __   | |  \\/ __ _ _ __ ___   ___ \n|    // _ \\ __| | | | '__| '_ \\  | | __ / _` | '_ ` _ \\ / _ \\\n| |\\ \\  __/ |_| |_| | |  | | | | | |_\\ \\ (_| | | | | | |  __/\n\\_| \\_\\___|\\__|\\__,_|_|  |_| |_|  \\____/\\__,_|_| |_| |_|\\___|
// 
// Quit
//\n\n  _____       _ _             _____ \n|  _  |     (_) |           |  __ \\  \n| | | |_   _ _| |_          | |  \\/ __ _ _ __ ___   ___   \n| | | | | | | | __|         | | __ / _  |  _   _ \\ / _ \\  \n\\ \\_/ / |_| | | |_          | |_\\ \\ (_| | | | | | |  __/  \n \\_/\\_\\\\__,_|_|\\__|          \\____/\\__,_|_| |_| |_|\\___|
// 
// Down Arrow
//\n\n              __          __\n              \\ \\        / /\n               \\ \\      / /\n                \\ \\    / / \n                 \\ \\  / / \n                  \\_\\/_/
//plane = ("(|__.-. |\n== == = _] +\n|")
//city = 	printf(" _____        _____    _________________ ________    _________\n|o o o|_______|    |__|               | | # # # |____|o o o o|\n|o o o|* * *|: ::|. . |               |o| # # # |. . |o o o o|\n|o o o|* * *|::  |. . | []  []  []  []|o| # # # |. . |o o o o|\n|o o o|**** |:  :| . .| []  []  []    |o| # # # |. . |o o o o|\n|_[]__|__[]_|_||_|__< |____________;;_|_|___[]__|_.|_|__[]___|");
//bomb = "<|>\n |_%s\n *",word
//
//every ascii character I used