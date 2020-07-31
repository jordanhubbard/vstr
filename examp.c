#include "vstr.h"
#include <stdio.h>
#include <ctype.h>

int i;

void eatnl() {
	register char ch;
	register int foundnum;

	while ((ch = getchar()) != '\n')
		if (isdigit(ch) || ch == '-') {
			ungetc(ch, stdin);
			scanf("%d", &i);
			foundnum = 1;
		}
	if (!foundnum)
		i = 0;
}

int main() {
		register p_vstr testv = 0;
		register char fname[40];
		char ch;
		struct cursor save;
		int going = 1, mode = FWD, cnt, chr;
		register FILE *fp;

		while (going) {
			printf("Command> ");
			ch = getchar();
			if (ch == '\n') {
				eatnl();
				continue;
			}
			eatnl();
			switch (ch) {

				case 'p':
					while ((ch = getchar()) != '~') {
						v_put(testv, ch, mode);
						if (mode == HERE)
							break;
					}
					eatnl();
					break;

				case 'g':
					if (mode == INS) {
						printf("Now in FWD mode\n");
						mode = FWD;
					}
					while ((ch = v_get(testv, mode)) !=
					  (mode == FWD ? ATEND : ATBEG)) {
						putchar(ch);
						if (mode == HERE)
							break;
					}
					printf("*END*\n");
					break;

				case 's':
					v_savecursor(testv, &save);
					printf("saved\n");
					break;

				case 'r':
					v_setcursor(testv, &save);
					printf("restored\n");
					break;

				case 'S':
					v_show(testv);
					break;

				case 'm':
					if (i < 0)
						i = v_move(testv, BACK, -i);
					else
						i = v_move(testv, FWD, i);
					printf("Moved %d positions\n", i);
					break;

				case 'd':
					printf("Deleted %d chars\n", v_delete(testv, i));
					break;

				case 'q':
					going = 0;
					break;

				case 'f':
					v_free(testv);
					testv = 0;
					break;

				case 'n':
					testv = new_vstr(i);
					printf("Address is %p\n", testv);
					break;

				case 'c':
					v_clear(testv, END);
					break;

				case 'R':
					v_rewind(testv);
					printf("Rewound\n");
					break;

				case 'A':
					v_append(testv);
					printf("Appended\n");
					break;

				case 'M':
					switch(i) {

						case 0:
							mode = FWD;
							printf("Forward\n");
							break;

						case 1:
							mode = BACK;
							printf("Back\n");
							break;

						case 2:
							mode = HERE;
							printf("Here\n");
							break;

						case 3:
							mode = INS;
							printf("Insert\n");
							break;

						default:
							printf("Unknown mode. Set to forward\n");
							mode = FWD;
							break;
					}
				break;

				case '>':
					cnt = 0;
					printf("To file name: ");
					scanf("%s", fname);
					eatnl();
					fp = fopen(fname, "w");
					if (!fp) {
						printf("Can't open %s for writing\n", fname);
						continue;
					}
					while ((ch = v_get(testv, mode)) !=
					  (mode == FWD ? ATEND : ATBEG)) {
						fputc(ch, fp);
						cnt++;
						if (mode == HERE)
							break;
					}
					printf("%d chars written\n", cnt);
					fclose(fp);
					break;

				case '<':
					cnt = 0;
					printf("From file name: ");
					scanf("%s", fname);
					eatnl();
					fp = fopen(fname, "r");
					if (!fp) {
						printf("Can't open %s for writing\n", fname);
						continue;
					}
					while((chr = fgetc(fp)) != EOF) {
						cnt++;
						v_put(testv, chr, FWD);
					}
					fclose(fp);
					printf("%d chars read, length of vstr is now %d\n",
					  cnt, v_length(testv, BEG));
					break;

				case '/':
					/* look ma, no statement */
					{
						register p_cursor cp;

					printf("Search for char: ");
					scanf("%c", &ch);
					if ((cp = v_find(testv, ch)) != NULL) {
						printf("Found.\n");
						v_setcursor(testv, cp);
					}
					} /* yuk. So I'm too lazy to declare my locals earlier.
						this is just a lousy test program.
						What do you want from me? Style? */
					break;

				default:
					printf("Unknown command: %c.\n", ch);
			}
	}
}
