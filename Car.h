#ifndef CAR_HE
#define CAR_HE

/*
	Structure Car.
*/
typedef struct Car 
{
	int id; // id 
	float posx; // pos axe x
	float posy; // pos axe y
	
	float speed; // vitesse
	float acc; // accélération
	
	char state; // état du véhicule

	char color; // couleur du véhicule

	int collision;
	float behind_speed;

	float toposx;

} Car;


/*
	Liste chainée de Car
*/
typedef struct element_list
{	
	Car* car;
	struct element_list* next;

} element_list;

typedef element_list* Cars_list;

/*
	Fonction qui crée un Car.
*/
Car* createCar(int id, int posx, int posy, float speed, float acc, char state, char color);
Cars_list addFirst(Cars_list list, Car* element);
Cars_list addCar(Cars_list list, Car* element);
Cars_list delCar(Cars_list list, Car* element);

// pour les scores : 


typedef struct element_score_list
{	
	int score;
	char nom[15];
	struct element_score_list* next;

} element_score_list;

typedef element_score_list* Score_list;

Score_list addScore(Score_list list, int score, char* nom);
Score_list delScore(Score_list list);

#endif