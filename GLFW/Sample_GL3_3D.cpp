#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>




using namespace std;

GLuint textureID[6];
struct VAO {
	GLuint VertexArrayID;
	GLuint VertexBuffer;
	GLuint ColorBuffer;
	GLuint TextureBuffer;
	GLuint TextureID;

	GLenum PrimitiveMode;
	GLenum FillMode;
	int NumVertices;
};
typedef struct VAO VAO;
struct GLMatrices {
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
	GLuint MatrixID;
	GLuint TexMatrixID; // For use with texture shader
} Matrices;

VAO *rectangle[6];
GLuint programID, textureProgramID;

float DOWNVEL=0.1;



/*********************************************************************************************/

int LEVEL_NUM=1;

int LIVES=5;
int SCORE=0;

const int BOARDWIDTH=20;
const int BOARDLENGTH=80;

float allcenters[BOARDLENGTH][BOARDWIDTH];


int VIEWS=1;
float VIEW_2X=130;
bool incrementview_2x=false;
bool decrementview_2x=false;
int lock=0;

double CURSOR_X,CURSOR_Y;
float STORE_MOUSE_X,STORE_MOUSE_Y;
bool ismousemove=false;
float hori_angle,ver_angle;
float final_hori_angle=0;
float final_ver_angle=0;
float VIEWLEN=30;
bool DEATH=false;


int LEVEL_I[]={0,51,36,13,0};
float level_posx[]={0,125,44,-3,-70};
float level_posy[]={0,-32,-10,-10,0};
float MAN_POS_X= level_posx[LEVEL_NUM];
float MAN_POS_Y= level_posy[LEVEL_NUM];




int BOARD_MAP[BOARDLENGTH][BOARDWIDTH] ={
	

	{0,0,0,0,0,0,0,1,0,4,0,1,0,0,0,0,0,0,0,0},//////
	{0,0,0,0,0,0,0,3,1,1,1,3,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,3,2,2,2,3,0,0,2,3,3,3,3,3},
	{0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,3,0,4,0,3},
	{0,4,0,4,2,0,0,0,0,0,2,0,2,0,0,3,0,0,0,3},
	{0,0,0,0,2,0,0,0,2,0,0,2,0,0,0,3,0,4,0,3},
	{1,1,0,0,4,0,0,4,0,4,0,0,0,0,0,3,0,0,0,3},
	{1,1,0,0,2,0,2,0,0,0,2,0,0,0,0,3,0,4,0,3},
	{1,0,0,0,0,4,0,0,0,4,0,0,0,0,0,3,0,0,0,3},
	{0,0,3,0,0,0,0,0,2,0,0,0,0,0,0,3,0,4,0,3},
	{4,4,0,1,1,1,0,0,0,4,1,1,1,0,0,0,0,0,0,0},
	{0,0,3,0,0,1,2,2,0,0,1,0,1,0,0,0,2,2,2,0},
	{0,0,0,0,0,0,2,2,0,0,1,1,1,2,2,3,3,2,3,3},
	
	{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},//
	{0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0},
	
	{0,0,0,0,0,0,0,2,0,0,3,2,2,0,3,3,2,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,2,0,0},
	{0,0,3,3,3,3,3,3,3,3,3,3,0,0,0,2,0,2,0,0},
	{0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,2,0,0},
	{0,4,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,2,0,0},
	{0,0,4,2,0,4,4,4,4,0,0,2,0,0,0,2,4,2,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,2,0,2,0,0},
	{0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,2,2,2,0,0},
	{0,0,0,0,0,0,0,0,2,0,0,2,0,0,0,2,0,2,0,0},
	{0,0,0,3,3,2,3,0,0,0,0,4,0,0,0,2,4,2,0,0},
	{0,2,0,2,2,2,2,2,3,0,0,4,0,0,0,2,0,2,0,0},
	{0,2,0,3,3,2,3,0,0,0,2,2,0,0,0,2,4,2,0,0},
	{0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,2,0,2,0,0},
	{0,3,3,0,0,0,0,0,0,2,2,2,2,0,0,2,4,2,0,0},
	{0,3,2,2,2,2,0,4,0,0,3,3,0,0,0,2,0,2,0,0},
	{0,3,3,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3,3,0},
	{0,0,0,0,0,0,0,3,0,0,3,3,3,3,3,3,3,3,3,0},
	
	{0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},//
	{0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0},
	
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,3,0,3,3,0,3,0,3,3,0,3,3,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3},
	{0,0,0,3,3,3,0,0,0,0,0,3,0,0,0,0,0,0,0,0},
	{0,3,3,3,3,3,3,3,0,3,3,0,3,3,0,0,0,0,0,3},
	{3,0,0,3,3,3,0,0,0,0,0,3,0,3,0,0,3,3,3,3},
	{0,3,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,0,0,0},
	{0,0,3,0,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
	
	{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},//
	{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0},

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,2,2,2,2,2,1,2,2,2,2,2,1,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0},
	{0,0,1,1,0,2,0,1,1,0,0,0,0,0,0,0,0,1,0,0},
	{1,1,1,0,0,0,2,1,1,1,0,1,1,0,1,0,1,1,0,0},
	{1,0,1,1,0,2,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0},
	{1,1,0,0,0,0,1,0,0,0,1,0,0,0,2,1,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,0},
	{0,0,0,0,0,0,0,0,1,1,0,0,2,0,0,0,0,1,0,0},
	{1,1,1,1,1,1,2,2,1,1,1,1,0,0,0,0,0,1,0,0},
	{1,0,1,0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,0},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};



int BOARD_ITEMS[BOARDLENGTH][BOARDWIDTH] ={
	

	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0},


	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0},



	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0},


	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,4,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0},


	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,3,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0},

	{0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};



typedef struct cubeColour{
	int c1,c2,c3,c4,c5,c6;
}cubeColour;


float camera4_x=0;
float camera4_y=2;

float camera5_x;
float camera5_y;
int SKYBOX=0;
float camera_rotation_angle=0;
float camera_angle=0;
int isincrementleft=0;
int isincrementright=0;
int isincrementdown=0;
int isincrementup=0;
int colour_state=0;
float MAN_POS_Z = 0.4;
float MAN_VEL_Z = 0.35;

float DECELERATION=MAN_VEL_Z/25.0;
float thigh_angle=0.0;
int thigh_direction=1;
bool isjump=0;
bool ismanincrement=0;
bool ismanrotate=0;
int  man_rotatedirection=1;
int man_movedirection=1;
float man_angle = 0;
bool jewel_status[(int)1e4];

vector<pair<float,float> > blockcenters;
vector<bool> rotate_status;
vector<pair<float,float> > woodbrickcenters;
vector<bool> brick_rotate_status;
vector<pair<int,bool> > greenblock_rotate_status;
vector<float> blockcenters_z;
vector<float> woodbrickcenters_z;
vector<float> greenblock_z;
vector<pair<float,float> > movingblockscent;
vector<float> movingblocks_z;
vector<pair<int,bool> >movingblocks_rotate_status;
vector<int> movingblocks_status;
vector<int> spikes_status;
vector<pair<float,float> > spikescenters;
vector<float> spikes_z;
vector<pair<float,float> > jewel_centers;
vector<float> jewel_z;
float coin_angle=0;


int isonblock=-1;


bool brick_fall_status[1000];
float brick_height[1000];
int brickfall_wait[1000];
bool brickfall_start[1000];
bool firsttime=true;
bool firsttime_items=true;

bool isfall=false;
bool CROSSOVER;


float store_z;

float GATE_POS_X=38;
float GATE_POS_Y=-9.5;


float leg_angle =0;
int leg_direction=1;


VAO * BLOCKS[1000000],*LOWER_BODY,*UPPER_BODY,*LEGS[2],*THIGHS1,*SHOE[2];
VAO *THIGHS[2],*NECK,*UPPER_HEAD,*LOWER_HEAD,*MIDDLE_HEAD,*SHOULDER[2],*HANDS[2];
VAO * WOODBRICK_1,*GREEN_BLOCK[3],*GATE_BASE1,*GATE_BASE2,*GATE_L_MIDDLE,*GATE_U_MIDDLE,*GATE_TOP;
VAO * GROUND,*SPIKES,*JEWEL[2];

/*******************************************************************************************/

float ExtractRed(int x)
{
	return ((float)x/1000000.0)/255.0;
}
float ExtractGreen(int x)
{
	x%=1000000;
	return ((float)x/1000.0)/255.0;
}

float ExtractBlue(int x)
{
	x%=1000;
	return (float)x/255.0;

}


void check_fall(void)
{
	int flag=0;
	
	for(int i=0;i<blockcenters.size();i++)
	{
		if(blockcenters[i].first-1.8<=MAN_POS_X && blockcenters[i].first+1.8>=MAN_POS_X && 
				MAN_POS_Y>=blockcenters[i].second-1.8 && MAN_POS_Y<=blockcenters[i].second+1.8 && MAN_POS_Z>=blockcenters_z[i]-0.7 && MAN_POS_Z<=blockcenters_z[i]+0.1)
		{
			MAN_POS_Z=blockcenters_z[i];
			flag=1;
		}
	}
	int temponblock=-1;
	for(int i=0;i<movingblockscent.size();i++)
	{
		if(movingblockscent[i].first-1.6<=MAN_POS_X && movingblockscent[i].first+1.6>=MAN_POS_X && 
				MAN_POS_Y>=movingblockscent[i].second-1.6 && MAN_POS_Y<=movingblockscent[i].second+1.6 && 
				MAN_POS_Z>=movingblocks_z[i]-0.1 && MAN_POS_Z <=movingblocks_z[i]+0.1)
		{
			MAN_POS_Z=movingblocks_z[i];
			isjump=0;
			flag=1;
			temponblock=i;
			break;
		}
	}


	
	if(temponblock!=-1)
		isonblock=temponblock;
	else
		isonblock=-1;
	if(isjump)
		flag=1;

	
	
	
	for(int i=0;i<woodbrickcenters.size();i++)
		if(woodbrickcenters[i].first-1.6<=MAN_POS_X && woodbrickcenters[i].first+1.6>=MAN_POS_X &&
				MAN_POS_Y>=woodbrickcenters[i].second-1.6 && MAN_POS_Y<=woodbrickcenters[i].second+1.6 && 
				MAN_POS_Z>=woodbrickcenters_z[i]-0.4 && MAN_POS_Z<=woodbrickcenters_z[i]+0.1)
		{
			if(brick_fall_status[i]){
				flag=0;
				break;
			}
			else flag=1;
			MAN_POS_Z=woodbrickcenters_z[i];

		}
	if(!flag)
		isfall=true;
	else 
		isfall=false;

}

void markbrick(void){
	for(int i=0;i<woodbrickcenters.size();i++)
		if(woodbrickcenters[i].first-1.6<=MAN_POS_X && woodbrickcenters[i].first+1.6>=MAN_POS_X && MAN_POS_Y>=woodbrickcenters[i].second-1.6 && MAN_POS_Y<=woodbrickcenters[i].second+1.6 && MAN_POS_Z>=woodbrickcenters_z[i] && MAN_POS_Z<=woodbrickcenters_z[i]+0.1)//MAN_POS_Z>=0.4 && MAN_POS_Z<=0.6)
			brickfall_start[i]=true;
}


void updatebrickwait(void){
	for(int i=0;i<woodbrickcenters.size();i++){
		if(brickfall_start[i])
			brickfall_wait[i]++;
	}
}


void checkbrickfall(void){
	for(int i=0;i<woodbrickcenters.size();i++)
		if(brickfall_wait[i]>60)
			brick_fall_status[i]=true;
}

void brickfall(void){
	for(int i=0;i<woodbrickcenters.size();i++)
		if(brick_fall_status[i])
			brick_height[i]--;
}



void check_spike_hit(int i,int j,int itt){

	if( spikescenters[itt].first-1.6<=MAN_POS_X && spikescenters[itt].first+1.6>=MAN_POS_X && MAN_POS_Y>=spikescenters[itt].second-1.6 && MAN_POS_Y<=spikescenters[itt].second+1.6 && MAN_POS_Z-1.8<=allcenters[(int)i][(int)j]+spikes_z[itt] && MAN_POS_Z-1.8>=allcenters[(int)i][(int)j]+spikes_z[itt]-2.5){
		
		MAN_POS_Z =0.4;
		MAN_POS_X = level_posx[LEVEL_NUM];
		MAN_POS_Y=level_posy[LEVEL_NUM];
		DOWNVEL=0.1;
		LIVES--;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	if(yoffset==1)
		VIEWLEN--;
	if(yoffset==-1)
		VIEWLEN++;
}

void collectCoins(void){

	for(int i=0;i<jewel_centers.size();i++)
		if(jewel_centers[i].first-1.6<=MAN_POS_X && jewel_centers[i].first+1.6>=MAN_POS_X && MAN_POS_Y>= jewel_centers[i].second-1.6 && MAN_POS_Y<=jewel_centers[i].second+1.6){
			if(!jewel_status[i]){
				SCORE+=10;
				cout<<"SCORE : "<<SCORE<<endl;
			}
			jewel_status[i]=true;

		}
}




/***************************************************************************************/



GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	return ProgramID;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

glm::vec3 getRGBfromHue (int hue)
{
	float intp;
	float fracp = modff(hue/60.0, &intp);
	float x = 1.0 - abs((float)((int)intp%2)+fracp-1.0);

	if (hue < 60)
		return glm::vec3(1,x,0);
	else if (hue < 120)
		return glm::vec3(x,1,0);
	else if (hue < 180)
		return glm::vec3(0,1,x);
	else if (hue < 240)
		return glm::vec3(0,x,1);
	else if (hue < 300)
		return glm::vec3(x,0,1);
	else
		return glm::vec3(1,0,x);
}













struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
	struct VAO* vao = new struct VAO;
	vao->PrimitiveMode = primitive_mode;
	vao->NumVertices = numVertices;
	vao->FillMode = fill_mode;
	glGenVertexArrays(1, &(vao->VertexArrayID)); 
	glGenBuffers (1, &(vao->VertexBuffer));
	glGenBuffers (1, &(vao->ColorBuffer));
	glBindVertexArray (vao->VertexArrayID);
	glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); 
	glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); 
	glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	return vao;
}

struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
	GLfloat* color_buffer_data = new GLfloat [3*numVertices];
	for (int i=0; i<numVertices; i++) {
		color_buffer_data [3*i] = red;
		color_buffer_data [3*i + 1] = green;
		color_buffer_data [3*i + 2] = blue;
	}

	return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}



struct VAO* create3DTexturedObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* texture_buffer_data, GLuint textureID, GLenum fill_mode=GL_FILL)
{
	struct VAO* vao = new struct VAO;
	vao->PrimitiveMode = primitive_mode;
	vao->NumVertices = numVertices;
	vao->FillMode = fill_mode;
	vao->TextureID = textureID;

	glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
	glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
	glGenBuffers (1, &(vao->TextureBuffer));  // VBO - textures

	glBindVertexArray (vao->VertexArrayID); // Bind the VAO
	glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices
	glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
	glVertexAttribPointer(
						  0,                  // attribute 0. Vertices
						  3,                  // size (x,y,z)
						  GL_FLOAT,           // type
						  GL_FALSE,           // normalized?
						  0,                  // stride
						  (void*)0            // array buffer offset
						  );

	glBindBuffer (GL_ARRAY_BUFFER, vao->TextureBuffer); // Bind the VBO textures
	glBufferData (GL_ARRAY_BUFFER, 2*numVertices*sizeof(GLfloat), texture_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
	glVertexAttribPointer(
						  2,                  // attribute 2. Textures
						  2,                  // size (s,t)
						  GL_FLOAT,           // type
						  GL_FALSE,           // normalized?
						  0,                  // stride
						  (void*)0            // array buffer offset
						  );

	return vao;
}







void draw3DObject (struct VAO* vao)
{
	glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);
	glBindVertexArray (vao->VertexArrayID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);
	glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}


void draw3DTexturedObject (struct VAO* vao)
{
	glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);
	glBindVertexArray (vao->VertexArrayID);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);
	glBindTexture(GL_TEXTURE_2D, vao->TextureID);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vao->TextureBuffer);
	glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint createTexture (const char* filename)
{
	GLuint TextureID;
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int twidth, theight;
	unsigned char* image = SOIL_load_image(filename, &twidth, &theight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D); // Generate MipMaps to use
	SOIL_free_image_data(image); // Free the data read from file after creating opengl texture
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess it up
	return TextureID;
}



VAO * createRectangle (GLuint textureID)
{
	// GL3 accepts only Triangles. Quads are not supported

	VAO * rect;
	static const GLfloat vertex_buffer_data [] = {
		-1000,-1000,0, // vertex 1
		1000,-1000,0, // vertex 2
		1000, 1000,0, // vertex 3

		1000, 1000,0, // vertex 3
		-1000, 1000,0, // vertex 4
		-1000,-1000,0  // vertex 1
	};

	static const GLfloat color_buffer_data [] = {
		1,0,0, // color 1
		0,0,1, // color 2
		0,1,0, // color 3

		0,1,0, // color 3
		0,0,0, // color 4
		1,0,0  // color 1
	};

	// Texture coordinates start with (0,0) at top left of the image to (1,1) at bot right
	static const GLfloat texture_buffer_data [] = {
		0,1, // TexCoord 1 - bot left
		1,1, // TexCoord 2 - bot right
		1,0, // TexCoord 3 - top right

		1,0, // TexCoord 3 - top right
		0,0, // TexCoord 4 - top left
		0,1  // TexCoord 1 - bot left
	};

	// create3DTexturedObject creates and returns a handle to a VAO that can be used later
	rect = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID, GL_FILL);
	return rect;
}


void update_skybox(void){

	if(SKYBOX==0){
		textureID[0]= createTexture("back.png");
		textureID[1]= createTexture("front.png");
		textureID[2]= createTexture("left.png");
		textureID[3]= createTexture("right.png");
		textureID[4]= createTexture("up.png");
		textureID[5]= createTexture("down.png");
	}
	else if(SKYBOX==1){
		textureID[0]= createTexture("back1.png");
		textureID[1]= createTexture("front1.png");
		textureID[2]= createTexture("left1.png");
		textureID[3]= createTexture("right1.png");
		textureID[4]= createTexture("up1.png");
		textureID[5]= createTexture("down1.png");
	}
	else if(SKYBOX==2){
		textureID[0]= createTexture("back2.png");
		textureID[1]= createTexture("front2.png");
		textureID[2]= createTexture("left2.png");
		textureID[3]= createTexture("right2.png");
		textureID[4]= createTexture("up2.png");
		textureID[5]= createTexture("down2.png");
	}
	else if(SKYBOX==3){
		textureID[0]= createTexture("back3.png");
		textureID[1]= createTexture("front3.png");
		textureID[2]= createTexture("left3.png");
		textureID[3]= createTexture("right3.png");
		textureID[4]= createTexture("up3.png");
		textureID[5]= createTexture("down3.png");
	}
	else if(SKYBOX==4){
		textureID[0]= createTexture("back4.png");
		textureID[1]= createTexture("front4.png");
		textureID[2]= createTexture("left4.png");
		textureID[3]= createTexture("right4.png");
		textureID[4]= createTexture("up4.png");
		textureID[5]= createTexture("down4.png");
	}


	for(int i=0;i<6;i++)
		rectangle[i]=createRectangle(textureID[i]);

}







void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_C:
				break;
			case GLFW_KEY_UP:
				if(VIEWS==2)decrementview_2x=false;
				isincrementup=0;
				break;
			case GLFW_KEY_DOWN:
				if(VIEWS==2)incrementview_2x=false;
				isincrementdown=0;
				break;
			case GLFW_KEY_LEFT:
				isincrementleft=0;
				break;
			case GLFW_KEY_RIGHT:
				isincrementright=0;
				break;
			case GLFW_KEY_W:
				ismanincrement=0;
				break;
			case GLFW_KEY_S:
				ismanincrement=0;
				break;
			case GLFW_KEY_A:
				ismanrotate=0;
				break;
			case GLFW_KEY_D:
				ismanrotate=0;
				break;
			default:
				break;
		}
	}
	else if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				quit(window);
				break;
			case GLFW_KEY_SPACE:
				if(!isfall){
					if(!isjump)
						store_z=MAN_POS_Z;
					isjump=1;
				}
				break;
			case GLFW_KEY_UP:
				isincrementup=1;
				if(VIEWS==2)decrementview_2x=true;
				break;
			case GLFW_KEY_DOWN:
				isincrementdown=1;
				if(VIEWS==2)incrementview_2x=true;
				break;
			case GLFW_KEY_LEFT:
				isincrementleft=1;
				break;
			case GLFW_KEY_RIGHT:
				isincrementright=1;
				break;
			case GLFW_KEY_W:
				man_movedirection=1;
				ismanincrement=1;
				break;
			case GLFW_KEY_S:
				man_movedirection=-1;
				ismanincrement=1;
				break;
			case GLFW_KEY_A:
				man_rotatedirection=1;
				ismanrotate=1;
				break;
			case GLFW_KEY_D:
				man_rotatedirection=-1;
				ismanrotate=1;
				break;
			case GLFW_KEY_T:
				VIEWS++;
				break;
			case GLFW_KEY_V:
				SKYBOX++;
				SKYBOX=SKYBOX%5;
				update_skybox();
				break;
			default:
				break;
		}
	}
}

void keyboardChar (GLFWwindow* window, unsigned int key)
{
	switch (key) {
		case 'Q':
		case 'q':
			quit(window);
			break;
		default:
			break;
	}
}

void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
	switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			if (action == GLFW_RELEASE)
			{
				ismousemove=false;
				lock=1;
			}
			else 
			{
				ismousemove=true;
				STORE_MOUSE_X=CURSOR_X;
				STORE_MOUSE_Y=CURSOR_Y;
			}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			if (action == GLFW_RELEASE) {
			}
			else
			{
			}
			break;
		default:
			break;
	}
}

void reshapeWindow (GLFWwindow* window, int width, int height)
{
	int fbwidth=width, fbheight=height;
	glfwGetFramebufferSize(window, &fbwidth, &fbheight);
	GLfloat fov = 90.0f;
	glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);
	Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 2000.0f);
	//Matrices.projection = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 500.0f);
}








//same tells that if all colurs are same.
VAO * createCube(cubeColour X,int same,float x1=-1.5,float  y1=-1.5,float z1=0 , float x2=1.5,float y2=-1.5,float z2=0 ,float x3=-1.5,float y3=-1.5,float z3=1 ,float x4=1.5,float y4=-1.5,float z4=1 ,float x5=-1.5,float y5=1.5,float z5=1 ,float x6=1.5,float y6=1.5,float z6=1 ,float x7=-1.5,float y7=1.5,float z7=0,float x8=1.5,float y8=1.5,float z8=0)
	
{
	VAO * temp_cube;
	if(same)
	{
		X.c2=X.c1;
		X.c3=X.c1;
		X.c4=X.c1;
		X.c5=X.c1;
		X.c6=X.c1;
	}
	//vertices from bottom out ,up out ,up in ,bottom in
	float r1=ExtractRed(X.c1);float g1=ExtractGreen(X.c1);float b1=ExtractBlue(X.c1);
	float r2=ExtractRed(X.c2);float g2=ExtractGreen(X.c2);float b2=ExtractBlue(X.c2);
	float r3=ExtractRed(X.c3);float g3=ExtractGreen(X.c3);float b3=ExtractBlue(X.c3);
	float r4=ExtractRed(X.c4);float g4=ExtractGreen(X.c4);float b4=ExtractBlue(X.c4);
	float r5=ExtractRed(X.c5);float g5=ExtractGreen(X.c5);float b5=ExtractBlue(X.c5);
	float r6=ExtractRed(X.c6);float g6=ExtractGreen(X.c6);float b6=ExtractBlue(X.c6);

	GLfloat g_vertex_buffer_data[] = {
		x1,y1,z1,x3,y3,z3,x5,y5,z5,x8,y8,z8,x1,y1,z1,x7,y7,z7,x4,y4,z4,x1,y1,z1,x2,y2,z2,x8,y8,z8,x2,y2,z2,x1,y1,z1,x1,y1,z1,x5,y5,z5,x7,y7,z7,x4,y4,z4,
		x3,y3,z3,x1,y1,z1,x5,y5,z5,x3,y3,z3,x4,y4,z4,x6,y6,z6,x2,y2,z2,x8,y8,z8,x2,y2,z2,x6,y6,z6,x4,y4,z4,x6,y6,z6,x8,y8,z8,x7,y7,z7,x6,y6,z6,x7,y7,z7,
		x5,y5,z5,x6,y6,z6,x5,y5,z5,x4,y4,z4
	};
	GLfloat g_color_buffer_data[] = {
		r1,g1,b1,r2,g2,b2,r3,g3,b3,r4,g4,b4,r1,g1,b1,r4,g4,b4,r2,g2,b2,r1,g1,b1,r1,g1,b1,r4,g4,b4,r1,g1,b1,r1,g1,b1,r1,g1,b1,r3,g3,b3,r4,g4,b4,r2,g2,b2,
		r2,g2,b2,r1,g1,b1,r3,g3,b3,r2,g2,b2,r2,g2,b2,r3,g3,b3,r1,g1,b1,r4,g4,b4,r1,g1,b1,r3,g3,b3,r2,g2,b2,r3,g3,b3,r4,g4,b4,r4,g4,b4,r3,g3,b3,r4,g4,b4,
		r3,g3,b3,r3,g3,b3,r3,g3,b3,r2,g2,b2
	};
	GLfloat g_color_buffer_data1[] = {

		r3,g3,b3,r3,g3,b3,r3,g3,b3,r4,g4,b4,r4,g4,b4,r4,g4,b4,r5,g5,b5,r5,g5,b5,r5,g5,b5,r4,g4,b4,r4,g4,b4,r4,g4,b4,r3,g3,b3,r3,g3,b3,r3,g3,b3,r5,g5,b5,
		r5,g5,b5,r5,g5,b5,r2,g2,b2,r2,g2,b2,r2,g2,b2,r1,g1,b1,r1,g1,b1,r1,g1,b1,r1,g1,b1,r1,g1,b1,r1,g1,b1,r6,g6,b6,r6,g6,b6,r6,g6,b6,r6,g6,b6,r6,g6,b6,
		r6,g6,b6,r2,g2,b2,r2,g2,b2,r2,g2,b2
	};

	if(colour_state)
		temp_cube=  create3DObject(GL_TRIANGLES, 36, g_vertex_buffer_data, g_color_buffer_data1, GL_FILL);
	else 
		temp_cube = create3DObject(GL_TRIANGLES, 36, g_vertex_buffer_data, g_color_buffer_data, GL_FILL);
	return temp_cube;
}


void draw ()
{


	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram (programID);



	if(MAN_POS_Z<=-20){
		MAN_POS_X=level_posx[LEVEL_NUM];
		MAN_POS_Y=level_posy[LEVEL_NUM];
		MAN_POS_Z=0.4;
		DOWNVEL=0.1;
		LIVES--;
		cout<<"LIVES : "<<LIVES<<endl;
	}

	if(LIVES==0){
		cout<<":: RIP :: "<<endl;
		DEATH=true;
	}

	if(ismanrotate)
	{
		man_angle+=man_rotatedirection*2;
		camera4_x=-4*sin(man_angle*M_PI/180.0);
		camera4_y=4*cos(man_angle*M_PI/180.0);
	}


	if(incrementview_2x)
		VIEW_2X+=0.5;
	else if(decrementview_2x)
		VIEW_2X-=0.5;

	if(ismousemove){
		hori_angle = ((STORE_MOUSE_X-CURSOR_X)/8.0)*M_PI/180.0;
		ver_angle = ((CURSOR_Y-STORE_MOUSE_Y)/8.0)*M_PI/180.0;
	}
	if(!ismousemove && lock){
		final_hori_angle += hori_angle;
		final_ver_angle +=ver_angle;
		hori_angle=0;
		ver_angle=0;
		lock=0;
	}


	if(VIEWS==0){
		glm::vec3 eye ( +60+40*cos(camera_rotation_angle*M_PI/180.0f),40*sin(camera_rotation_angle*M_PI/180.0f),40*sin(camera_angle*M_PI/180.0));
		glm::vec3 target (+60, 0, 0);
		glm:: vec3 up(0,0,1);
		Matrices.view = glm::lookAt( eye, target, up );
	}
	else if(VIEWS==1)
		Matrices.view = glm::lookAt( glm:: vec3(MAN_POS_X,MAN_POS_Y,22), glm::vec3(MAN_POS_X,MAN_POS_Y,-1), glm::vec3(-1,0,0) );
	else if(VIEWS==2)
		Matrices.view = glm::lookAt( glm:: vec3(VIEW_2X,-12,30), glm::vec3(VIEW_2X-60,-12,0), glm::vec3(-1,0,1) );
	else if(VIEWS==3)
	Matrices.view = glm::lookAt( glm:: vec3(MAN_POS_X+camera4_x/4.0,MAN_POS_Y+camera4_y/4.0,MAN_POS_Z+4), glm::vec3(MAN_POS_X+camera4_x,MAN_POS_Y+camera4_y,MAN_POS_Z+4), glm::vec3(0,0,1) );
	else if(VIEWS==4)
		Matrices.view = glm::lookAt( glm:: vec3(MAN_POS_X-2*camera4_x,MAN_POS_Y-2*camera4_y,MAN_POS_Z+11), glm::vec3(MAN_POS_X+camera4_x,MAN_POS_Y+camera4_y,MAN_POS_Z+11), glm::vec3(0,0,1) );
	else if(VIEWS==5)
		Matrices.view = glm::lookAt( glm:: vec3(90+VIEWLEN*cos(final_ver_angle+ver_angle)*cos( final_hori_angle+ hori_angle),-10+VIEWLEN*cos(final_ver_angle+ver_angle)*sin(final_hori_angle+ hori_angle),VIEWLEN*sin(final_ver_angle+ver_angle)), glm::vec3(90,-10,0), glm::vec3(0,0,1));


	VIEWS=VIEWS%6;
	
	
	
	glm::mat4 VP = Matrices.projection * Matrices.view;
	glm::mat4 MVP;


	/*if(MAN_POS_X+5<GATE_POS_X)
		CROSSOVER=true;
	else
		CROSSOVER=false;
*/



	//cout<<MAN_POS_X<<" "<<MAN_POS_Y<<endl;




	if(isjump && !isfall)
	{
		cout<<"Fallig man"<<endl;
		MAN_POS_Z+=MAN_VEL_Z;
		MAN_VEL_Z-=DECELERATION;
	
		if(MAN_POS_Z<=store_z){
			isjump=0;
			MAN_POS_Z=store_z;
			MAN_VEL_Z=0.35;
		}
	}





	if(isfall && !isjump){
		MAN_POS_Z-=DOWNVEL;
		//DOWNVEL+=0.02;
	}
	if(ismanincrement)
	{
		MAN_POS_X +=man_movedirection*.15*cos((man_angle+90)*M_PI/180.0);
		MAN_POS_Y +=man_movedirection*.15*sin((man_angle+90)*M_PI/180.0);
	}


	glm::mat4 rotateman = glm::rotate((float)(man_angle*M_PI/180.0f), glm::vec3(0,0,1)); 
	if(isincrementright)
		camera_rotation_angle+=1;
	if(isincrementleft)
		camera_rotation_angle-=1;

	if(isincrementdown)
		camera_angle-=1;
	if(isincrementup)
		camera_angle+=1;




	//cout<<"Position : "<<MAN_POS_Z<<" "<<" is fall "<<isfall<<endl;

	collectCoins();


	int itt=0;
	int itt2=0;
	for(float i = 0 ; i < BOARDLENGTH ; i++)
		for(float j = 0 ; j < BOARDWIDTH ; j++){
			if(BOARD_ITEMS[(int)i][(int)j]){
				if(firsttime_items)
				{
					if(BOARD_ITEMS[(int)i][(int)j]==1){
						spikescenters.push_back(make_pair((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2));
						spikes_z.push_back(rand()%3-1);
						spikes_status.push_back(rand()%2);
					}
					else if(BOARD_ITEMS[(int)i][(int)j]==2){
						jewel_centers.push_back(make_pair((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2));
						jewel_z.push_back(rand()%180);
					}
				}
				Matrices.model = glm::mat4(1.0f);
				if(BOARD_ITEMS[(int)i][(int)j]==1 ){
					
					if(i>=LEVEL_I[LEVEL_NUM]){
					if(spikes_z[itt]>=-2){
						
					check_spike_hit(i,j,itt);
					for(int k=0;k<2;k++)
						for(int l=0;l<2;l++){
						glm::mat4 tspike1=glm::translate(glm::vec3((i*1.6-1.6*BOARDLENGTH/2)*2-0.8+l*1.6,(j*1.6-1.6*BOARDWIDTH/2)*2-.8+k*1.6,allcenters[(int)i][(int)j]+spikes_z[itt]));
						glm:: mat4 Sspike1=glm::scale(glm::vec3(0.5,0.5,1));
						Matrices.model = tspike1*Sspike1;
						MVP = VP * Matrices.model;
						glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
						draw3DObject(SPIKES);
						}
				}
					if(spikes_status[itt]){
						if(spikes_z[itt]>=0.8)
							spikes_status[itt]=0;
						else 
							spikes_z[itt]+=0.03;
					}
					else{
						if(spikes_z[itt]<=-4)
							spikes_status[itt]=1;
						else
							spikes_z[itt]-=0.03;
					}
					}
					itt++;
				}
				else if(BOARD_ITEMS[(int)i][(int)j]==2){
				
					if(i>=LEVEL_I[LEVEL_NUM] && !jewel_status[itt2]){
						for(int k=0;k<2;k++){
							Matrices.model = glm::mat4(1.0f);
							glm:: mat4 T1=glm::translate(glm::vec3(jewel_centers[itt2].first,jewel_centers[itt2].second,5+2*cos(M_PI*jewel_z[itt2]/180.0)));
							glm::mat4 R1 = glm::rotate((float)(coin_angle*M_PI/180.0f), glm::vec3(0,0,1));
							Matrices.model *= T1*R1;
							MVP = VP * Matrices.model;
							glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
							draw3DObject(JEWEL[k]);
							coin_angle+=0.1;
							jewel_z[itt2]++;
						}
					}
					itt2++;

				}
			}
		}
	firsttime_items=false;




	for(int i=4;i>LEVEL_NUM;i--)
	if(level_posx[i]-5>=MAN_POS_X)
			LEVEL_NUM++;


	markbrick();
	updatebrickwait();
	checkbrickfall();
	brickfall();


	int it=0;
	int it2=0;
	int it3=0;
	int it4=0;
	for(float i = 0 ; i < BOARDLENGTH ; i++)
		for(float j = 0 ; j < BOARDWIDTH ; j++){
			if(BOARD_MAP[(int)i][(int)j]){
				if(firsttime)
				{
					if(BOARD_MAP[(int)i][(int)j]==1){
						blockcenters.push_back(make_pair((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2));
						rotate_status.push_back(rand()&1);
						blockcenters_z.push_back(0.4);
						allcenters[(int)i][(int)j]=0.4;
					}
					else if(BOARD_MAP[(int)i][(int)j]==2){
						woodbrickcenters.push_back(make_pair((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2));
						brick_rotate_status.push_back(rand()&1);
						woodbrickcenters_z.push_back(0.4);
						allcenters[(int)i][(int)j]=0.4;
					}
					else if(BOARD_MAP[(int)i][(int)j]==3){
						blockcenters.push_back(make_pair((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2));
						greenblock_rotate_status.push_back(make_pair(rand()%2,rand()&1));
						blockcenters_z.push_back(0.4);
						greenblock_z.push_back(0.4);
						allcenters[(int)i][(int)j]=0.4;
					}
					else if(BOARD_MAP[(int)i][(int)j]==4){
						movingblockscent.push_back(make_pair((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2));
						movingblocks_rotate_status.push_back(make_pair(rand()%2,rand()&1));
						movingblocks_z.push_back(rand()%10-5);
						movingblocks_status.push_back(rand()%2);
						allcenters[(int)i][(int)j]=movingblocks_z.back();
					}

				}
				Matrices.model = glm::mat4(1.0f);
				if(BOARD_MAP[(int)i][(int)j]==1){
					
				if(i>=LEVEL_I[LEVEL_NUM]){
					glm::mat4 translateCube=glm::translate(glm::vec3((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2,0));
					if(rotate_status[it]){
						glm::mat4 rotatecube = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));
						Matrices.model *= (translateCube)*rotatecube;
					}
					else
						Matrices.model *= (translateCube);
					MVP = VP * Matrices.model;
					glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
					draw3DObject(BLOCKS[0]);
					}
					it++;
				}
				else if(BOARD_MAP[(int)i][(int)j]==2){
					
					if(i>=LEVEL_I[LEVEL_NUM]){
					glm::mat4 scaleCube=glm::scale(glm::vec3(0.9,0.3,0.5));
					glm::mat4 translateCube=glm::translate(glm::vec3((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2,brick_height[it2]));
					glm :: mat4 positionCube=glm::translate(glm::vec3(0,-1.0,0));
					for(int l=1;l<=3;l++){
						if(brick_rotate_status[it2]){
							glm::mat4 rotatecube = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));
							Matrices.model = (translateCube)*rotatecube*positionCube*scaleCube;
						}
						else
							Matrices.model = (translateCube)*positionCube*scaleCube;
						MVP = VP * Matrices.model;
						glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
						draw3DObject(WOODBRICK_1);
						positionCube=glm::translate(glm::vec3(0,-1+l,0));
					}
					}
					it2++;
				}
				else if(BOARD_MAP[(int)i][(int)j]==3){
					
					if(i>=LEVEL_I[LEVEL_NUM]){
					glm::mat4 translateCube=glm::translate(glm::vec3((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2,0));
					if(greenblock_rotate_status[it3].second){
						glm::mat4 rotatecube = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));
						Matrices.model *= (translateCube)*rotatecube;
					}
					else
						Matrices.model *= (translateCube);
					MVP = VP * Matrices.model;
					glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
					draw3DObject(GREEN_BLOCK[greenblock_rotate_status[it3].first]);
					}
					it3++;
				}
				else if(BOARD_MAP[(int)i][(int)j]==4){
					
					if(i>=LEVEL_I[LEVEL_NUM]){
					glm::mat4 tCube=glm::translate(glm::vec3((i*1.6-1.6*BOARDLENGTH/2)*2,(j*1.6-1.6*BOARDWIDTH/2)*2,movingblocks_z[it4]-0.4));
					if(movingblocks_rotate_status[it4].second){
						glm::mat4 rotatecube = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));
						Matrices.model *= (tCube)*rotatecube;
					}
					else
						Matrices.model *= (tCube);
					MVP = VP * Matrices.model;
					glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
					draw3DObject(GREEN_BLOCK[movingblocks_rotate_status[it4].first]);
				
					if(movingblocks_status[it4]){
						if(movingblocks_z[it4]>=5.4)
							movingblocks_status[it4]=0;
						else 
							movingblocks_z[it4]+=0.1;
					}
					else{
						if(movingblocks_z[it4]<=-8)
							movingblocks_status[it4]=1;
						else
							movingblocks_z[it4]-=0.1;
					}
					allcenters[(int)i][(int)j]=movingblocks_z[it4];
					if(isonblock==it4 && !isjump)
						MAN_POS_Z=movingblocks_z[it4];
					}
					it4++;
				}
			}
		}
	firsttime=false;



	cout<<endl;
	
	//cout<<"is jump before"<<isjump<<endl;
	check_fall();
	//cout<<"is jump after"<<isjump<<endl;

	if(LEVEL_NUM==4){
		Matrices.model = glm::mat4(1.0f);
		glm:: mat4 tground1=glm::translate(glm::vec3(-110,16,-6));
		glm:: mat4 tscale1 =glm::scale(glm::vec3(8,1,5));
		Matrices.model *= tground1*tscale1;
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		draw3DObject(GROUND);



		Matrices.model = glm::translate(glm::vec3(0,13,0))*Matrices.model;
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		draw3DObject(GROUND);

		Matrices.model = glm::mat4(1.0f);
		tground1=glm::translate(glm::vec3(-121,22,-6));
		tscale1 =glm::scale(glm::vec3(1,5.5,5));
		Matrices.model *= tground1*tscale1;
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		draw3DObject(GROUND);



	}



///////////////////////////////////////////////////////////


	for(int i=0;i<LEVEL_NUM;i++){

	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 tbase1=glm::translate(glm::vec3(GATE_POS_X-i*47-(i==2)*27 -(i==3)*26,GATE_POS_Y-5+(i==3)*7,0));
	Matrices.model *= tbase1;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_BASE1);

	MVP*=glm::translate(glm::vec3(0,10,0));
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_BASE1);

	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 tbase2=glm::translate(glm::vec3(GATE_POS_X-i*47-(i==2)*27-(i==3)*26,GATE_POS_Y-5+(i==3)*7,0));
	Matrices.model *= tbase2;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_BASE2);

	MVP*=glm::translate(glm::vec3(0,10,0));
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_BASE2);


	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 tmid1=glm::translate(glm::vec3(GATE_POS_X-i*47-(i==2)*27-(i==3)*26,GATE_POS_Y-5+(i==3)*7,0));
	Matrices.model *= tmid1;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_L_MIDDLE);

	Matrices.model *= glm::translate(glm::vec3(0,10,0));
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_L_MIDDLE);

	Matrices.model = glm::mat4(1.0f);
	tmid1=glm::translate(glm::vec3(GATE_POS_X-i*47-(i==2)*27-(i==3)*26,GATE_POS_Y-5+(i==3)*7,0));
	glm::mat4 rotatemid1 = glm::rotate((float)(180*M_PI/180.0f), glm::vec3(0,0,1));
	Matrices.model *= tmid1*rotatemid1;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_L_MIDDLE);

	tmid1=glm::translate(glm::vec3(0,10,0));
	Matrices.model = tmid1*Matrices.model;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_L_MIDDLE);


	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 tmid2=glm::translate(glm::vec3(GATE_POS_X-i*47-(i==2)*27-(i==3)*26,GATE_POS_Y-5+(i==3)*7,0));
	Matrices.model *= tmid2;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_U_MIDDLE);

	tmid2=glm::translate(glm::vec3(GATE_POS_X-i*47-(i==2)*27-(i==3)*26,5+GATE_POS_Y+(i==3)*7,0));
	Matrices.model = tmid2;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_U_MIDDLE);


	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 Stop=glm::scale(glm::vec3(1,1.2,1));
	glm:: mat4 Ttop=glm::translate(glm::vec3(GATE_POS_X-i*47-(i==2)*27-(i==3)*26,GATE_POS_Y+(i==3)*7,0));
	Matrices.model *= Ttop*Stop;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(GATE_TOP);

	}

/////////////////////////////////////////////////////////////////////////////


	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 translatebody1=glm::translate(glm::vec3(0,0,3));
	glm:: mat4 tbody1=glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
	Matrices.model *= tbody1*rotateman*(translatebody1);
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(LOWER_BODY);

	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 translatebody2=glm::translate(glm::vec3(0,0,3.5));
	glm:: mat4 tbody2=glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
	Matrices.model *= tbody2*rotateman*(translatebody2);
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(UPPER_BODY);


	if(ismanincrement)
	{
		thigh_angle+=1*thigh_direction;
		leg_angle+=2.0/3.0*leg_direction;
	}

	if(leg_angle>=20)
		leg_direction=-1;
	if(leg_angle<=-20)
		leg_direction=1;


	
	if(thigh_angle>=30)
		thigh_direction=-1;
	if(thigh_angle<=-30)
		thigh_direction=1;


	for(int i=0;i<2;i++)
	{
		Matrices.model = glm::mat4(1.0f);
		glm:: mat4 Tleg1=glm::translate(glm::vec3(0,0,-1));
		glm::mat4 Rleg1 = glm::rotate((float)(2*(.5-i)*leg_angle*M_PI/180.0f), glm::vec3(1,0,0));
		glm:: mat4 Tleg2=glm::translate(glm::vec3(0,0,-1));
		glm::mat4 Rleg2 = glm::rotate((float)(2*(.5-i)*thigh_angle*M_PI/180.0f), glm::vec3(1,0,0));
		glm:: mat4 Tleg3=glm::translate(glm::vec3(-.3+i*.6,0,3));
		glm:: mat4 Tleg4=glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
		Matrices.model *= Tleg4*rotateman*Tleg3*Rleg2*Tleg2*Rleg1*Tleg1;
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		draw3DObject(LEGS[i]);
	}
	
	for(int i=0;i<2;i++)
	{
		Matrices.model = glm::mat4(1.0f);
		glm:: mat4 Tshoe1= glm::translate(glm::vec3(0,0,-1));
		glm::mat4 Rshoe1 = glm::rotate((float)(2*(.5-i)*leg_angle*M_PI/180.0f), glm::vec3(1,0,0));
		glm:: mat4 Tshoe2= glm::translate(glm::vec3(0,0,-1));
		glm::mat4 Rshoe2 = glm::rotate((float)(2*(.5-i)*thigh_angle*M_PI/180.0f), glm::vec3(1,0,0));
		glm:: mat4 Tshoe3= glm::translate(glm::vec3(-.3+i*.6,-0.3,3));
		glm:: mat4 Tshoe4= glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
		Matrices.model *=  Tshoe4*rotateman*Tshoe3*Rshoe2*Tshoe2*Rshoe1*Tshoe1;
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		draw3DObject(SHOE[i]);
	}




	for(int i=0;i<2;i++)
	{
		Matrices.model = glm::mat4(1.0f);
		glm:: mat4 Tthigh1=glm::translate(glm::vec3(0,0,-1));
		glm::mat4 Rthigh1 = glm::rotate((float)(2*(.5-i)*thigh_angle*M_PI/180.0f), glm::vec3(1,0,0));
		glm:: mat4 Tthigh011=glm::translate(glm::vec3(-.3+i*0.6,0,3));
		glm:: mat4 Tthigh01=glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
		Matrices.model *= Tthigh01*rotateman*Tthigh011*Rthigh1*Tthigh1;
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		draw3DObject(THIGHS[i]);

	}

	for(int i=0;i<2;i++)
	{
		Matrices.model = glm::mat4(1.0f);
		glm::mat4 Rshould1 = glm::rotate((float)(-2*(.5-i)*thigh_angle*M_PI/180.0f), glm::vec3(1,0,0));
		glm:: mat4 Tshould1=glm::translate(glm::vec3(-0.8+i*1.6,0,4.5));
		glm:: mat4 Tshould4= glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
		Matrices.model *=  Tshould4*rotateman*Tshould1*Rshould1;
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		draw3DObject(SHOULDER[i]);
	}


	for(int i=0;i<2;i++)
	{
		Matrices.model = glm::mat4(1.0f);
		glm:: mat4 Thands1 = glm::translate(glm::vec3(0,0,-1));
		glm:: mat4 Rhands1 = glm::rotate((float)(-2*(.5-i)*leg_angle*M_PI/180.0f), glm::vec3(1,0,0));
		glm:: mat4 Thands2 = glm::translate(glm::vec3(0,0,-0.8));
		glm:: mat4 Rhands2 = glm::rotate((float)(-2*(.5-i)*thigh_angle*M_PI/180.0f), glm::vec3(1,0,0));
		glm:: mat4 Thands3 = glm::translate(glm::vec3(-0.8+i*1.6,0,2.7+1.8));
		glm:: mat4 Thands4=glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
		Matrices.model *= Thands4*rotateman*Thands3*Rhands2*Thands2*Rhands1*Thands1;
		MVP = VP * Matrices.model;
		glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
		draw3DObject(HANDS[i]);
	}


	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 Tneck=glm::translate(glm::vec3(0,0,4.5));
	glm:: mat4 Tneck01=glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
	Matrices.model *= Tneck01*rotateman*(Tneck);
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(NECK);

	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 Thead1=glm::translate(glm::vec3(0,0,4.7));
	glm:: mat4 Thead01=glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
	Matrices.model *= Thead01*rotateman*(Thead1);
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(LOWER_HEAD);

	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 Thead2=glm::translate(glm::vec3(0,0,4.9));
	glm:: mat4 Thead02=glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
	Matrices.model *=Thead02*rotateman*(Thead2);
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(MIDDLE_HEAD);

	Matrices.model = glm::mat4(1.0f);
	glm:: mat4 Thead3=glm::translate(glm::vec3(0,0,5.2));
	glm:: mat4 Thead03=glm::translate(glm::vec3(MAN_POS_X,MAN_POS_Y,MAN_POS_Z));
	Matrices.model *= Thead03*rotateman*(Thead3);
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	draw3DObject(UPPER_HEAD);



	glUseProgram(textureProgramID);




	
	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translateRectangle = glm::translate (glm::vec3(0, -1000, 0));        // glTranslatef
	glm:: mat4 Rect1 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(1,0,0));
	glm:: mat4 Rect2 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));
	glm:: mat4 Rect3 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));
	Matrices.model *= (translateRectangle)*Rect3*Rect2*Rect1;
	MVP = VP * Matrices.model;							////LEFT
	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
	draw3DTexturedObject(rectangle[2]);


	Matrices.model = glm::mat4(1.0f);
	translateRectangle = glm::translate (glm::vec3(1000, 0, 0));        // glTranslatef
	Rect1 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(1,0,0));
	Rect2 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));
	Rect3 = glm::rotate((float)(180*M_PI/180.0f), glm::vec3(0,0,1));
	Matrices.model *= (translateRectangle)*Rect3*Rect2*Rect1;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);        ////FRONT
	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
	draw3DTexturedObject(rectangle[1]);



	Matrices.model = glm::mat4(1.0f);
	translateRectangle = glm::translate (glm::vec3(-1000, 0, 0));        // glTranslatef
	Rect1 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(1,0,0));
	Rect2 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));			///BACK
	Rect3 = glm::rotate((float)(0*M_PI/180.0f), glm::vec3(0,0,1));
	Matrices.model *= (translateRectangle)*Rect3*Rect2*Rect1;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
	draw3DTexturedObject(rectangle[0]);




	Matrices.model = glm::mat4(1.0f);
	translateRectangle = glm::translate (glm::vec3(0, 0,-1000));        // glTranslatef
	Rect1 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(1,0,0));
	Rect2 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));		////DOWN
	Rect3 = glm::rotate((float)(-90*M_PI/180.0f), glm::vec3(0,1,0));
	Matrices.model *= (translateRectangle)*Rect3*Rect2*Rect1;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
	draw3DTexturedObject(rectangle[5]);


	Matrices.model = glm::mat4(1.0f);
	translateRectangle = glm::translate (glm::vec3(0, 0,1000));        // glTranslatef
	glm:: mat4 Rect0 = glm::rotate((float)(180*M_PI/180.0f), glm::vec3(0,0,1));
	Rect1 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(1,0,0));
	Rect2 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));		//UP
	Rect3 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,1,0));
	Matrices.model *= (translateRectangle)*Rect3*Rect2*Rect1*Rect0;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
	draw3DTexturedObject(rectangle[4]);




	Matrices.model = glm::mat4(1.0f);
	translateRectangle = glm::translate (glm::vec3(0, 1000, 0));        // glTranslatef
	Rect1 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(1,0,0));
	Rect2 = glm::rotate((float)(90*M_PI/180.0f), glm::vec3(0,0,1));
	Rect3 = glm::rotate((float)(-90*M_PI/180.0f), glm::vec3(0,0,1)); //RIGHT
	Matrices.model *= (translateRectangle)*Rect3*Rect2*Rect1;
	MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);
	draw3DTexturedObject(rectangle[3]);




}

GLFWwindow* initGLFW (int width, int height)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval( 1 );
	glfwSetFramebufferSizeCallback(window, reshapeWindow);
	glfwSetWindowSizeCallback(window, reshapeWindow);
	glfwSetWindowCloseCallback(window, quit);
	glfwSetKeyCallback(window, keyboard);      
	glfwSetCharCallback(window, keyboardChar);  
	glfwSetMouseButtonCallback(window, mouseButton);
	return window;
}

void initGL (GLFWwindow* window, int width, int height)
{

	glActiveTexture(GL_TEXTURE0);
	
	
	textureID[0]= createTexture("back.png");
	textureID[1]= createTexture("front.png");
	textureID[2]= createTexture("left.png");
	textureID[3]= createTexture("right.png");
	textureID[4]= createTexture("up.png");
	textureID[5]= createTexture("down.png");

	
	
	
	//if(textureID == 0 )
	//	cout << "SOIL loading error: '" << SOIL_last_result() << "'" << endl;

	textureProgramID = LoadShaders( "TextureRender.vert", "TextureRender.frag" );
	Matrices.TexMatrixID = glGetUniformLocation(textureProgramID, "MVP");
	for(int i=0;i<6;i++)
		rectangle[i]=createRectangle(textureID[i]);






	for(int i = 0 ; i < BOARDLENGTH ; i++)
		for(int j = 0 ; j < BOARDWIDTH ; j++){
			if(BOARD_MAP[i][j] && rand()%6==0)
				BOARD_ITEMS[i][j]=2;
		}
	for(int i=0;i<=LEVEL_I[2];i++)
		for(int j=0;j<BOARDWIDTH;j++)
			if(BOARD_MAP[i][j] & rand()%6==0)
				BOARD_ITEMS[i][j]=1;
	
	
	
	cubeColour X;
	X.c1=205102029;X.c2=255165079;X.c3=205102029;X.c4=205102029;
	BLOCKS[0]=createCube(X,0);

	//BLUE STUFF
	X.c1=205;
	LOWER_BODY =createCube(X,1,-0.6,-0.4,0, 0.6,-0.4,0, -0.6,-0.4,0.5 ,0.6,-0.4,0.5, -0.6,0.4,0.5, 0.6,0.4,0.5, -0.6,0.4,0, 0.6,0.4,0);
	for(int i=0;i<2;i++)
		THIGHS[i]=createCube(X,1,-0.2,-0.4,0, 0.2,-0.4,0, -0.2-i*.1,-0.4,1.2 ,0.3,-0.4,1.2, -0.2-i*.1,0.4,1.2, 0.3,0.4,1.2, -0.2,0.4,0, 0.2,0.4,0);





	//SKIN STUFF
	X.c1=255160122;
	for(int i=0;i<2;i++)
		LEGS[i]=createCube(X,1,-0.2,-0.2,0, 0.2,-0.2,0, -0.2,-0.2,1.0 ,0.2,-0.2,1.0, -0.2,0.2,1.0, 0.2,0.2,1.0, -0.2,0.2,0 ,0.2,0.2,0);


	NECK=createCube(X,1, -0.2,-0.1,0, 0.2,-0.1,0, -0.1,-0.1,0.2 ,0.1,-0.1,.2, -0.1,0.1,.2, 0.1,0.1,.2, -0.2,0.1,0 ,0.2,0.1,0);

	MIDDLE_HEAD=createCube(X,1, -0.2,-0.2,0, 0.2,-0.2,0, -0.2,-0.2,0.3 ,0.2,-0.2,.3, -0.2,0.2,.3, 0.2,0.2,.3, -0.2,0.2,0 ,0.2,0.2,0);
	LOWER_HEAD=createCube(X,1,  -0.1,-0.1,0, 0.1,-0.1,0, -0.2,-0.2,0.2 ,0.2,-0.2,.2, -0.2,0.2,.2, 0.2,0.2,.2, -0.1,0.1,0 ,0.1,0.1,0);

	HANDS[0]=createCube(X,1,-0.1,-0.1,0, 0.1,-0.1,0, -0.15,-0.15,1.0 ,0.15,-0.15,1.0, -0.15,0.15,1.0, 0.15,0.15,1.0, -0.1,0.1,0 ,0.1,0.1,0);
	HANDS[1]=createCube(X,1,-0.1,-0.1,0, 0.1,-0.1,0, -0.15,-0.15,1.0 ,0.15,-0.15,1.0, -0.15,0.15,1.0, 0.15,0.15,1.0, -0.1,0.1,0 ,0.1,0.1,0);


	X.c1=170;
	SHOULDER[0]=createCube(X,1,-0.2,-0.2,-0.8, 0.2,-0.2,-0.8, -0.1,-0.2,-0.2 ,0.2,-0.2,0, -0.1,0.2,-0.2, 0.2,0.2,0, -0.2,0.2,-0.8 ,0.2,0.2,-0.8);
	SHOULDER[1]=createCube(X,1,-0.2,-0.2,-0.8, 0.2,-0.2,-0.8, -0.2,-0.2,0 ,0.1,-0.2,-0.2, -0.2,0.2,0, 0.1,0.2,-0.2, -0.2,0.2,-0.8 ,0.2,0.2,-0.8);


	//BLACK STUFF
	X.c1=0;
	UPPER_BODY =createCube(X,1,-0.6,-0.4,0, 0.6,-0.4,0, -0.65,-0.4,1 ,0.65,-0.4,1, -0.65,0.4,1, 0.65,0.4,1, -0.6,0.4,0, 0.6,0.4,0);

	UPPER_HEAD=createCube(X,1,-0.2,-0.2,0, 0.2,-0.2,0, -0.1,-0.2,0.2 ,0.1,-0.2,.2, -0.1,0.2,.2, 0.1,0.2,.2, -0.2,0.2,0 ,0.2,0.2,0);
	
	for(int i=0;i<2;i++)
		SHOE[i]=createCube(X,1, -0.25,0,-0.4, 0.25,0,-0.4, -0.25,0,0 ,0.25,0,0, -0.25,.7,0, 0.25,0.7,0, -0.25,0.8,-0.4 ,0.25,0.8,-0.4);



	//WOOD COLOUR
	X.c1=107066038;X.c2=139090043;X.c3=X.c1;X.c4=X.c1;
	WOODBRICK_1=createCube(X,0);

	//GREEN BLOCKS
	X.c1=156102031;X.c4=X.c1;X.c2=139000;X.c3=100000;
	GREEN_BLOCK[0]=createCube(X,0, -1.5,-1.5,-1 ,1.5,-1.5,-1 ,-1.5,-1.5,1 ,1.5,-1.5,1 ,-1.5,1.5,1 ,1.5,1.5,1 ,-1.5,1.5,-1,1.5,1.5,-1);
	X.c1=156102031;X.c4=X.c1;X.c2=128000;X.c3=48128020;	
	GREEN_BLOCK[1]=createCube(X,0, -1.5,-1.5,-1 ,1.5,-1.5,-1 ,-1.5,-1.5,1 ,1.5,-1.5,1 ,-1.5,1.5,1 ,1.5,1.5,1 ,-1.5,1.5,-1,1.5,1.5,-1);
	X.c1=156102031;X.c4=X.c1;X.c2=139000;X.c3=34139034;	
	GREEN_BLOCK[2]=createCube(X,0, -1.5,-1.5,-1 ,1.5,-1.5,-1 ,-1.5,-1.5,1 ,1.5,-1.5,1 ,-1.5,1.5,1 ,1.5,1.5,1 ,-1.5,1.5,-1,1.5,1.5,-1);


	X.c1=156102031;	
	GROUND=createCube(X,1, -1.5,-1.5,-1 ,1.5,-1.5,-1 ,-1.5,-1.5,1 ,1.5,-1.5,1 ,-1.5,1.5,1 ,1.5,1.5,1 ,-1.5,1.5,-1,1.5,1.5,-1);


	X.c1=139090043;X.c2=139069019;X.c3=X.c2;X.c4=X.c1;

	GATE_BASE1 = createCube(X,0, -1.5,-1.5,1 ,1.5,-1.5,1, -1,-1,7 ,1,-1,7, -1,1,7, 1,1,7, -1.5,1.5,1 ,1.5,1.5,1);

	X.c1=132132132;X.c2=82082082;X.c3=X.c2;X.c4=X.c1;
	GATE_BASE2 = createCube(X,1, 1.5,-1.5,1, 1.5,1.5,1, 1.5-(8.0/3.0),-1,3 ,1.5-(8.0/3.0),1,3,  4,-.7,2.5, 4,.7,2.5, 4,-.7,1 ,4,.7,1);
	
	X.c1=132132132;X.c2=82082082;X.c3=X.c2;X.c4=X.c1;
	GATE_L_MIDDLE =createCube(X,0, -1,0,7, -.5,-1,7, -1,0,9 ,-0.5,-1,9, 1,0,9, .5,-1,9, 1,0,7 ,.5,-1,7);
	
	X.c1=107066038;X.c2=139090043;X.c3=X.c1;X.c4=X.c1;
	GATE_U_MIDDLE =createCube(X,1, -1,0,9, 0,-1,9, -1,0,10 ,0,-1,10, 0,1,10 ,1,0,10 , 0,1,9, 1,0,9 );
	
	X.c1=166166166;X.c2=102102102;X.c3=X.c2;X.c4=X.c1;
	GATE_TOP = createCube(X,0, -0.8,-5,10, -0.8,5,10, -1,-5.5,11 ,-1,5.5,11, 1,-5.5,11 ,1,5.5,11, 0.8,-5,10, 0.8,5,10 );


	//GRAY
	X.c1=84084084;
	SPIKES = createCube(X,1, -0.5,0.5,0, -0.5,-0.5,0, 0,0,3 ,0,0,3, 0,0,3 ,0,0,3, 0.5,0.5,0, 0.5,-0.5,0 );



	//X.c2=255114086;X.c1=800000000;X.c3=X.c2;X.c4=X.c1;
	
	X.c2= 218191000;X.c1=255255000;X.c3=X.c2;X.c4=X.c1;
	JEWEL[0] = createCube(X,0, -0.5,0.5,0, -0.5,-0.5,0, 0,0,1.5 ,0,0,1.5, 0,0,1.5 ,0,0,1.5, 0.5,0.5,0, 0.5,-0.5,0 );
	JEWEL[1] = createCube(X,0, -0.5,0.5,0, -0.5,-0.5,0, 0,0,-1.5 ,0,0,-1.5, 0,0,-1.5 ,0,0,-1.5, 0.5,0.5,0, 0.5,-0.5,0 );


	programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
	reshapeWindow (window, width, height);
	glClearColor (126.0/255.0,192.0/255.0,238.0/255.0, 1); // R, G, B, A
	glClearDepth (1.0f);
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main (int argc, char** argv)
{
	int width = 1400;
	int height = 720;
	GLFWwindow* window = initGLFW(width, height);
	initGL (window, width, height);
	double last_update_time = glfwGetTime(), current_time;
	while (!glfwWindowShouldClose(window)) 
	{
		glfwGetCursorPos(window,&CURSOR_X,&CURSOR_Y);
		glfwSetScrollCallback(window, scroll_callback);
		draw();
		if(DEATH)
			quit(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
		current_time = glfwGetTime(); // Time in seconds
		if ((current_time - last_update_time) >= 0.5) { // atleast 0.5s elapsed since last frame
			// do something every 0.5 seconds ..
			last_update_time = current_time;
		}
	}
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
