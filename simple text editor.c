#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxNoOfLinees 200
#define maxLineKaLen 100//maximim line lenth that can be...

char buff[maxNoOfLinees][maxLineKaLen];// buff to store text content
int currLine=0;// curr line position

void menuKoDisplayKaro() {
    printf("\nTEXT EDITOR MENU:\n");
    printf("1--> create new file\n");
    printf("2--> open file\n");
    printf("3--> save file\n");
    printf("4--> write text\n");
    printf("5--> cut line\n");
    printf("6--> copy line\n");
    printf("7--> paste line\n");
    printf("8--> find text\n");
    printf("9--> replace text\n");
    printf("10--> underline text\n");
    printf("11--> rename file\n");
    printf("12--> display file data\n");
    printf("13--> insert line\n");
    printf("14--> delete line\n");
    printf("15--> exit\n");

}

void newFileBanao(const char *file) {
    FILE *fpointer=fopen(file,"w");
    if (fpointer==NULL) {
        printf("error creating file\n");
        return;
    }
    fclose(fpointer);
    printf("file '%s' created\n",file);
}

void fileOpenKaro(const char *file) {
    FILE *fpointer=fopen(file,"r");
    if (fpointer==NULL) {
        printf("error opening file %s\n",file);
        return;
    }

    currLine=0;
    while (fgets(buff[currLine], maxLineKaLen, fpointer)!= NULL) {
        currLine++;
    }
    fclose(fpointer);
    printf("file '%s' opened\n",file);
}

void fileSaveKaro(const char *file) {
    FILE *fpointer=fopen(file,"w");
    if (fpointer==NULL) {
        printf("error saving file %s\n",file);
        return;
    }
    int i;
    for (i=0;i<currLine;i++) {
        printf(fpointer,"%s\n",buff[i]);
    }
    fclose(fpointer);
    printf("file '%s' saved successfully\n",file);
}

void textLikho() {
    char txt[maxLineKaLen];
    //printf("enter text: ");
    //printf("enter '.exit' to exit the program");
    fgets(txt,maxLineKaLen,stdin);
    
    //strcmp two strings ko compare karta hai
    //.exit exit comand hai
    if (strcmp(txt,".exit\n")==0) {
       return;
    }

    //check karta hai if text length is >= maximum line length-1
    //that -1 is for \0 null character
    if (strlen(txt)>= maxLineKaLen-1) {
        printf("warning!!!!\n");
        txt[maxLineKaLen - 2]='\0'; 
        //last second place ko \0 assign karta hai
    }

    // Shift existing lines down if necessary
    if (currLine==maxNoOfLinees) {
        printf("cannot add more lines!!\n");
        return;
    }
    int i;
    for (i=currLine;i>0;i--) {
        strcpy(buff[i],buff[i-1]);//pushes the content 1 line down to make place
    }
    //yeh loop buffer me space banane ke liye
    strcpy(buff[0],txt);
    currLine++;
}

void fileDsplyKaro() {
  if (currLine==0) {
    printf("nothing to display\n");
    return;
  }
    int i;
  for (i=currLine-1;i>=0; i--) {
    printf("%s\n",buff[i]);
  }
}

void lineinsrtKaro() {
    if (currLine==maxNoOfLinees) {
        printf("warning!cannot insert more lines!\n");
        return;
    }

    int lineinsrtKaro=currLine;// by default, it will insert at the end

    //giving user the choice to insert line wherever they wish to
    printf("enter line no to insert before:");
    printf("enter 0 for end/-1 to cancel");
    int lineno;
    scanf("%d", &lineno);

    if (lineno==-1) {
        return;
    } else if (lineno<-1||lineno>=currLine+1) {
        printf("invalid num\n");
        return;
    }
    
    //ternary operator
    lineinsrtKaro=lineno>=0?lineno:currLine; 

    int i;
    //pushing existing content down
    for (i=currLine;i>lineinsrtKaro;i--) {
        strcpy(buff[i],buff[i-1]);
    }

    //first char. position ko \0 assign karta hai, creates an empty string
    buff[lineinsrtKaro][0]='\0';
    currLine++;
}

void lineDlteKaro() {
    if (currLine==0) {
        printf("nothing to delete\n");
        return;
    }

    int lineDlteKaro;
    printf("enter line num to delete:");
    scanf("%d",&lineDlteKaro);
    int i;
    if (lineDlteKaro>0&&lineDlteKaro<=currLine) {
        //rem lines ko upar push karta hain
        for (i=lineDlteKaro;i<currLine-1;i++) {
            strcpy(buff[i],buff[i+1]);
        }
        currLine--;
    } else {
        printf("invalid\n");
    }
}

int clipboardlines=-1; //clipboard ka line index
char clipboardData[maxLineKaLen];//copied/cut content ko store karta hai

void cutLine() {
    if (currLine==0) {
        printf("nothing to cut\n");
        return;
    }

    clipboardlines=0;
    strcpy(clipboardData,buff[0]);
    int i;
    for (i=0;i<currLine-1;i++) {
        strcpy(buff[i],buff[i+1]);
    }
    currLine--;
}

void cpylinee() {
    if (currLine==0) {
        printf("nothing to copy\n");
        return;
    }

    clipboardlines=currLine-1;
    strcpy(clipboardData,buff[clipboardlines]);
}

void pstelinee() {
    if (clipboardlines==-1) {
        printf("nothing to paste\n");
        return;
    }

    if (currLine==maxNoOfLinees) {
        printf("warning!buffer is full\n");
        return;
    }

    int i;
    for (i=currLine;i>0;i--) {
        strcpy(buff[i],buff[i-1]);
    }

    strcpy(buff[0],clipboardData);
    currLine++;
}

int fndtxt(const char *text) {
    int i;
    for (i=0;i<currLine;i++) {
        //strstr ek pointer ko return karta hai, within the buff[i]
        if (strstr(buff[i],text) != NULL) {
            return i; 
        }
    }
    return -1; 
}

void rplcetxt(const char *oldtxt, const char *newtxt) {
    int x=0;
    int i;
    for (i=0;i<currLine;i++) {
        char *ptr=strstr(buff[i],oldtxt);
        if (ptr!=NULL) {
            int oldtxtlength=strlen(oldtxt);
            int newtxtlength=strlen(newtxt);
            int remlen=strlen(ptr+oldtxtlength);

            
            if (newtxtlength>oldtxtlength&&remlen<newtxtlength-oldtxtlength) {
                printf("warning!replacement text too long\n");
            }

           

            // for (int j = 0; j <= remlen; j++) {
            // buff[i][ptr + newtxtlength - j] = buff[i][ptr + oldtxtlength - j];
            // }
            // Shift remaining content to make space for new text
            memmove(ptr+newtxtlength,ptr+oldtxtlength,remlen+1); // +1 null terminator ke liye hai

            //final replacing
            strncpy(ptr,newtxt,newtxtlength);
            x=1;
        }
    }
    if (x) {
        printf("text replaced\n");
    } else {
        printf("text '%s' not found\n", oldtxt);
    }
}

void filenamechnge(const char *oldname, const char *newname) {
    if (rename(oldname, newname)==0) {
        printf("file '%s' renamed to '%s'\n", oldname, newname);
    } else {
        printf("errorr!!!\n");
    }
}

//  #define colourCode 10
//  
//  char colorChoice[10];
//  printf("\nAvailable Color Codes:\n");
//  printf("  - black   (code: 30)\n");
//  printf("  - red     (code: 31)\n");
//  printf("  - green   (code: 32)\n");
//  printf("  - yellow  (code: 33)\n");
//  printf("  - blue    (code: 34)\n");
//  printf("  - magenta (code: 35)\n");
//  printf("  - cyan    (code: 36)\n");
//  printf("  - white   (code: 37)\n");
//
//  printf("Enter color code for the colour you want: ");
//  scanf("%s", colorChoice);


void underlineText(const char *text) {
    printf("%s\n",text);//original text ko print karo
    int i;
    for (i=0;text[i]!='\0';i++) {
        printf("-");
    }
    printf("\n");
}

int main() {
    char file[maxLineKaLen];
    int choice;
	menuKoDisplayKaro();
	
    while (1) {
    printf("\nenter your choice:");        
        scanf("%d", &choice);
        while (getchar()!='\n');

        switch (choice) {
            case 1:
                printf("Enter file for new file: ");
                scanf("%s",file);
                newFileBanao(file);
                break;
            case 2:
                printf("Enter file to open: ");
                scanf("%s",file);
                fileOpenKaro(file);
                break;
            case 3:
                if (strlen(file)==0) {
                    printf("No file opened yet.\n");
                } 
                else {
                    fileSaveKaro(file);
                }
                break;
            case 4:
                printf("enter '.exit' to exit the program\n");
            	printf("enter text:");
                textLikho();
                break;
            case 5:
                cutLine();
                break;
            case 6:
                cpylinee();
                break;
            case 7:
                pstelinee();
                break;
            case 8: {
                char text[maxLineKaLen];
                printf("enter text to find:");
                scanf("%s", text);
                int linefound=fndtxt(text);
                if (linefound==-1) {
                    printf("text '%s' not found\n", text);
                } 
                else {
                    printf("text found in line %d\n",linefound+1);
                }
                break;
            }
            case 9: {
                char oldText[maxLineKaLen];
                char newText[maxLineKaLen];
                printf("enter text to replace:");
                scanf("%s",oldText);
                printf("enter new text:");
                scanf("%s",newText);
                rplcetxt(oldText,newText);
                break;
            }
            case 10: {
                if (currLine==0) {
                printf("nothing to underline\n");
                } else {
                int i;
                for (i=0;i<currLine;i++) {
                underlineText(buff[i]);
                }
                }
                break;
            }
            case 11: {
                if (strlen(file)==0) {
                    printf("no file opened yet\n");
                } 
                else {
                    char newName[maxLineKaLen];
                    printf("enter new file:");
                    scanf("%s",newName);
                    filenamechnge(file,newName);
                    strcpy(file,newName);
                }
                break;
            }
            case 12:
                fileDsplyKaro();
                break;
            case 13:
                lineinsrtKaro();
                break;
            case 14:
                lineDlteKaro();
                break;
            default:
                printf("choice not valid\n");
        }
    }

    return 0;
}




