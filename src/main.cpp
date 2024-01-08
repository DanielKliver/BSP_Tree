#include<iostream>
#include<vector>
#include<ctime>
#define NUM_OF_POLYGONS 10

using namespace std;

//точка, содержащая координаты x и y
typedef class point
{
	public:

		int x;
		int y;
		int z;

}point;

//полигон, представляющий из себя прямую с двумя точками (концы)
typedef class polygon
{
	public:

		int id_num;
		point one;
		point two;
		point three;

}polygon;

//класс с функциями генерации рандомных чисел и заполнения вектора полигонамиы
class random
{
	public:
		//статический метод рандомного заполнения массива полигонов
		static vector<polygon> rand_input(vector<polygon> list_of_polygons)
		{
			srand(time(NULL));
			list_of_polygons.resize(NUM_OF_POLYGONS);
			for(int i = 1; i <= NUM_OF_POLYGONS; i++)
			{

				list_of_polygons[i].id_num = i;
				//заполнение точек координатами по x, y, z в диапазоне от 1 до 200
				list_of_polygons[i].one.x = rand()%(200 - 1 + 1) + 1;
				list_of_polygons[i].one.y = rand()%(200 - 1 + 1) + 1;
				list_of_polygons[i].one.z = 1;
				list_of_polygons[i].two.x = rand()%(200 - 1 + 1) + 1;
				list_of_polygons[i].two.y = rand()%(200 - 1 + 1) + 1;
				list_of_polygons[i].two.z = 1;
				list_of_polygons[i].three.x = list_of_polygons[i].one.x;
				list_of_polygons[i].three.y = list_of_polygons[i].one.y;
				list_of_polygons[i].three.z = 0;

			}

			return list_of_polygons;
		}
};

int main()
{
	//создаём список полигонов;
	vector<polygon> list_of_polygons;
	//создаём несколько полигонов и заполняем их рандомными значениями
	list_of_polygons = random::rand_input(list_of_polygons);
	//полигоны есть! осталось пропустить их через bsp комплилятор и построить bsp дерево.


	return 0;
}