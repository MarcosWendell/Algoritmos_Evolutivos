#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>

#define	WINDOW_WIDTH 1205
#define WINDOW_HEIGHT 550
#define FPS 25


int tf = 1, p = 0;
int counter;

pthread_mutex_t pause,save_exit;

int points[12][5];
int nests[12][50][2];
unsigned char fields[12][100][100];
int generation = 0;

void writeText(int x, int y, char*str, int n){
	for(int i = 0 ;i  < n; i++){
		glRasterPos2f(x+10*i,y);
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
		glVertex2f(-600,225);
		glVertex2f(605,225);
		//comecar -23
		glVertex2f(-600,176);
		glVertex2f(605,176);

		glVertex2f(-600,-25);
		glVertex2f(605,-25);
		//comecar -274
		glVertex2f(-600,-75);
		glVertex2f(605,-75);
		//comecar -599
		glVertex2f(-399,-275);
		glVertex2f(-399,225);
		//comecar -398
		glVertex2f(-198,-275);
		glVertex2f(-198,225);
		//comecar -197
		glVertex2f(3,-275);
		glVertex2f(3,225);
		//comecar 4
		glVertex2f(204,-275);
		glVertex2f(204,225);
		//comecar 205
		glVertex2f(405,-275);
		glVertex2f(405,225);
		//comecar 406
	glEnd();

	char texto[] = {'g','e','r','a','c','a','o',':',' ','0'+((generation/100)%10),'0'+((generation/10)%10),'0'+ (generation % 10),'\0'};
	char fitness[] = {'f','i','t','n','e','s','s',':',' ','0','0','0','\0'};

	writeText(-590,255,texto,12);
/*	writeText(-590,210,"ninho 1",7);
	fitness[10] = '0'+((points[0]/100)%10);
	fitness[11] = '0'+((points[0]/10)%10);
	fitness[12] = '0'+(points[0] % 10);
	writeText(-590,190,fitness,12);
	writeText(-388,210,"ninho 2",7);
	fitness[10] = '0'+((points[1]/100)%10);
	fitness[11] = '0'+((points[1]/10)%10);
	fitness[12] = '0'+(points[1] % 10);
	writeText(-388,190,fitness,12);
	writeText(-187,210,"ninho 3",7);
	fitness[10] = '0'+((points[2]/100)%10);
	fitness[11] = '0'+((points[2]/10)%10);
	fitness[12] = '0'+(points[2] % 10);
	writeText(-187,190,fitness,12);
	writeText(14,210,"ninho 4",7);
	fitness[10] = '0'+((points[3]/100)%10);
	fitness[11] = '0'+((points[3]/10)%10);
	fitness[12] = '0'+(points[3] % 10);
	writeText(14,190,fitness,12);
	writeText(215,210,"ninho 5",7);
	fitness[10] = '0'+((points[4]/100)%10);
	fitness[11] = '0'+((points[4]/10)%10);
	fitness[12] = '0'+(points[4] % 10);
	writeText(215,190,fitness,12);
	writeText(416,210,"ninho 6",7);
	fitness[10] = '0'+((points[0]/100)%10);
	fitness[11] = '0'+((points[0]/10)%10);
	fitness[12] = '0'+(points[0] % 10);
	writeText(416,190,fitness,12);

	writeText(-590,-40,"ninho 7",7);
	fitness[10] = '0'+((points[0]/100)%10);
	fitness[11] = '0'+((points[0]/10)%10);
	fitness[12] = '0'+(points[0] % 10);
	writeText(-590,-60,fitness,12);
	writeText(-388,-40,"ninho 8",7);
	fitness[10] = '0'+((points[0]/100)%10);
	fitness[11] = '0'+((points[0]/10)%10);
	fitness[12] = '0'+(points[0] % 10);
	writeText(-388,-60,fitness,12);
	writeText(-187,-40,"ninho 9",7);
	fitness[10] = '0'+((points[0]/100)%10);
	fitness[11] = '0'+((points[0]/10)%10);
	fitness[12] = '0'+(points[0] % 10);
	writeText(-187,-60,fitness,12);
	writeText(14,-40,"ninho 10",8);
	fitness[10] = '0'+((points[0]/100)%10);
	fitness[11] = '0'+((points[0]/10)%10);
	fitness[12] = '0'+(points[0] % 10);
	writeText(14,-60,fitness,12);
	writeText(215,-40,"ninho 11",8);
	fitness[10] = '0'+((points[0]/100)%10);
	fitness[11] = '0'+((points[0]/10)%10);
	fitness[12] = '0'+(points[0] % 10);
	writeText(215,-60,fitness,12);
	writeText(416,-40,"ninho 12",8);
	fitness[10] = '0'+((points[0]/100)%10);
	fitness[11] = '0'+((points[0]/10)%10);
	fitness[12] = '0'+(points[0] % 10);
	writeText(416,-60,fitness,12);
*/
	glPointSize(2);
	glColor3f(0,1,0);
	glBegin(GL_POINTS);
	for(int i = 0; i <100;i++){
		for(int j = 0; j < 100;j++){
			if(fields[0][i][j])
				glVertex2f(2*i-599,2*j-23);
			if(fields[1][i][j])
				glVertex2f(2*i-398,2*j-23);
			if(fields[2][i][j])
				glVertex2f(2*i-197,2*j-23);
			if(fields[3][i][j])
				glVertex2f(2*i+4,2*j-23);
			if(fields[4][i][j])
				glVertex2f(2*i+205,2*j-23);
			if(fields[5][i][j])
				glVertex2f(2*i+406,2*j-23);
			if(fields[6][i][j])
				glVertex2f(2*i-599,2*j-274);
			if(fields[7][i][j])
				glVertex2f(2*i-398,2*j-274);
			if(fields[8][i][j])
				glVertex2f(2*i-197,2*j-274);
			if(fields[9][i][j])
				glVertex2f(2*i+4,2*j-274);
			if(fields[10][i][j])
				glVertex2f(2*i+205,2*j-274);
			if(fields[11][i][j])
				glVertex2f(2*i+406,2*j-274);
		}
	}
	glEnd();
	glPointSize(3);

	glColor3f(1,0,0);
	glBegin(GL_POINTS);
		for(int i = 0; i < 50; i++){
			glVertex2f(2*nests[0][i][0]-599,2*nests[0][i][1]-23);
			glVertex2f(2*nests[1][i][0]-398,2*nests[1][i][1]-23);
			glVertex2f(2*nests[2][i][0]-197,2*nests[2][i][1]-23);
			glVertex2f(2*nests[3][i][0]+4,2*nests[3][i][1]-23);
			glVertex2f(2*nests[4][i][0]+205,2*nests[4][i][1]-23);
			glVertex2f(2*nests[5][i][0]+406,2*nests[5][i][1]-23);
			glVertex2f(2*nests[6][i][0]-599,2*nests[6][i][1]-274);
			glVertex2f(2*nests[7][i][0]-398,2*nests[7][i][1]-274);
			glVertex2f(2*nests[8][i][0]-197,2*nests[8][i][1]-274);
			glVertex2f(2*nests[9][i][0]+4,2*nests[9][i][1]-274);
			glVertex2f(2*nests[10][i][0]+205,2*nests[10][i][1]-274);
			glVertex2f(2*nests[11][i][0]+406,2*nests[11][i][1]-274);
		}
	glEnd();

	glColor3f(0,0,0);

	glFlush();
}

void draw(int value){
	glutPostRedisplay();
	if(tf == 1)
		glutTimerFunc(FPS,draw,0);
}

void keyboard(unsigned char key, int x, int y){
	if(key == 'g'){
			tf = !tf;
			if(tf == 1)
				glutTimerFunc(FPS,&draw,0);
	}else if(key == 'p'){
			p = !p;
			if(p == 1)
				pthread_mutex_lock(&pause);
			else
				pthread_mutex_unlock(&pause);
	}else if(key == 'q'){
		pthread_mutex_lock(&save_exit);
		pthread_mutex_unlock(&pause);
	}
}

void init(){
	glClearColor(1,1,1,1);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-600,605,-275,275);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0,0,0);
	glPointSize(3);
	glutPostRedisplay();
}

void *UI(void *arg){
	glutInit(&counter,arg);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(120,109);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Evolutivos");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	init();
	glutTimerFunc(FPS,&draw,0);
	glutMainLoop();

	return 0;
}
