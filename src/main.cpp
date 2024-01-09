#include<iostream>
#include<vector>
#include<ctime>
#define NUM_OF_POLYGONS 10

using namespace std;


typedef class node
{
public:

	int id_of_polygon;
	node* parent;
	node* front;
	node* back;

	node(int id_of_polygon)
	{
		this->id_of_polygon = id_of_polygon;
		parent = NULL;
		front = NULL;
		back = NULL;
	}


}node;
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
		for (int i = 0; i < NUM_OF_POLYGONS; i++)
		{

			list_of_polygons[i].id_num = i;
			//заполнение точек координатами по x, y, z в диапазоне от 1 до 200
			list_of_polygons[i].one.x = rand() % (200 - 1 + 1) + 1;
			list_of_polygons[i].one.y = rand() % (200 - 1 + 1) + 1;
			list_of_polygons[i].one.z = 1;
			list_of_polygons[i].two.x = rand() % (200 - 1 + 1) + 1;
			list_of_polygons[i].two.y = rand() % (200 - 1 + 1) + 1;
			list_of_polygons[i].two.z = 1;
			list_of_polygons[i].three.x = list_of_polygons[i].one.x;
			list_of_polygons[i].three.y = list_of_polygons[i].one.y;
			list_of_polygons[i].three.z = 0;

		}

		return list_of_polygons;
	}
};

typedef class ret_poly
{
	public:
	vector<polygon> front;
	vector<polygon> back;

	ret_poly(vector<polygon> front, vector<polygon> back)
	{
		this->front = front;
		this->back = back;
	}


}ret_poly;

class BSP_CMP
{
private:

	int* vector_product(int* edge1, int* edge2)
	{
		int* normal = new int[3];
		//векторное произведение
		normal[0] = edge1[1] * edge2[2] - edge2[1] * edge1[2];
		normal[1] = edge1[2] * edge2[0] - edge2[2] * edge1[0];
		normal[2] = edge1[0] * edge2[1] - edge2[0] * edge1[1];

		return normal;
	}

	int* find_normal(polygon list_of_polygons)
	{
		//скорее всего нуждается в рефакторинге
		int* edge1 = new int[3];
		int* edge2 = new int[3];
		//вычисление двух векторов полигона для дальнейшего поиска нормали 
		//и составления уравнения плоскости
		edge1[0] = list_of_polygons.two.x - list_of_polygons.one.x;
		edge1[1] = list_of_polygons.two.y - list_of_polygons.one.y;
		edge1[2] = list_of_polygons.two.z - list_of_polygons.one.z;

		edge1[0] = list_of_polygons.three.x - list_of_polygons.one.x;
		edge1[1] = list_of_polygons.three.y - list_of_polygons.one.y;
		edge1[2] = list_of_polygons.three.z - list_of_polygons.one.z;
		//////////////////////////////////////////////////////////////////
		int* normal = new int[3];
		//векторное произведение, получаем нормаль
		normal = vector_product(edge1, edge2);
		delete[] edge1;
		delete[] edge2;

		return normal;
	}

	int check_position(int* normal, polygon main_polygon, polygon list_of_polygons)
	{
		float value = normal[0] * (list_of_polygons.one.x - main_polygon.one.x) + normal[1] * (list_of_polygons.one.y - main_polygon.one.y + normal[2] * (list_of_polygons.one.z - 1));
		int result = 0;
		if (value < 0)
		{
			result--;
		}
		else if (value > 0)
		{
			result++;
		}
		value = normal[0] * (list_of_polygons.two.x - main_polygon.one.x) + normal[1] * (list_of_polygons.two.y - main_polygon.one.y + normal[2] * (list_of_polygons.two.z - 1));
		if (value < 0)
		{
			result--;
		}
		else if (value > 0)
		{
			result++;
		}

		return result;


	}
	ret_poly classify_polygon(vector<polygon> list_of_polygons, vector<polygon> front_list, vector<polygon> back_list)
	{
		//вычислить два ребра и выполнить операцию векторного произведения
		int* normal = new int[3];
		normal = find_normal(list_of_polygons[0]);
		//сохраняем необходимые компоненты первого полигона и удаляем его из списка
		polygon main_polygon;
		main_polygon.one.x = list_of_polygons[0].one.x;
		main_polygon.one.y = list_of_polygons[0].one.y;
		list_of_polygons.erase(list_of_polygons.begin());
		//составить уравнение плоскости вида 0*(x-1)-7*(y-12)+0*(z-1)
		//подставлять в него значения первых двух точек каждого полигона, добавлять в back/front
		//удалить первый полигон из вектора, вернуть его
		while (list_of_polygons.size() != 0)
		{
			int result = check_position(normal, main_polygon, list_of_polygons[0]);
			//в данном случае никаких багов в 3д реализции BSP не будет, если одна из точек будет лежать в дной плоскости с полигоном
			//поэтому это можно не учитывать
			if (result == 1)
			{
				result++;
			}
			else if (result == -1)
			{
				result--;
			}

			switch (result)
			{
				//ниже полигона
			case -2:
				back_list.push_back(list_of_polygons[0]);
				break;
				//выше полигона
			case +2:
				front_list.push_back(list_of_polygons[0]);
				break;

			case 0:
				//на нём
				back_list.push_back(list_of_polygons[0]);
				front_list.push_back(list_of_polygons[0]);
				break;
			default:
				cout << "Ошибка результата разбиения" << endl;
				break;
			}
			list_of_polygons.erase(list_of_polygons.begin());

		}
		ret_poly vec(front_list, back_list);
		return vec;

	}


public:

	BSP_CMP(node root, vector<polygon> list_of_polygons)
	{
		if (root.id_of_polygon == 0)
		{
			root.id_of_polygon = list_of_polygons[0].id_num;
		}

		vector<polygon> front_list;
		vector<polygon> back_list;
		//составляем backlist и frontlist. после чего можно сделать рекурсивный вызов конструктора
		//с передачей корня и front/back листов
		ret_poly vec = classify_polygon(list_of_polygons, front_list, back_list);
		front_list = vec.front;
		back_list = vec.back;
		


	}

};
int main()
{
	//создаём список полигонов;
 	vector<polygon> list_of_polygons;

	//создаём несколько полигонов и заполняем их рандомными значениями
	list_of_polygons = random::rand_input(list_of_polygons);
	//полигоны есть! осталось пропустить их через bsp комплилятор и построить bsp дерево.
	node root(0);
	BSP_CMP(root, list_of_polygons);

	return 0;
}