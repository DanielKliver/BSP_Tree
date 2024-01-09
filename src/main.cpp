#include<iostream>
#include<vector>
#include<ctime>
#define NUM_OF_POLYGONS 200

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

	ret_poly classify_polygon(vector<polygon> list_of_polygons, vector<polygon> front_list, vector<polygon> back_list)
	{
		//вычислить два ребра и выполнить операцию векторного произведения
		int edge1[3];
		int edge2[3];
		//вычисление двух векторов полигона для дальнейшего поиска нормали 
		//и составления уравнения плоскости
		edge1[0] = list_of_polygons[0].two.x - list_of_polygons[0].one.x;
		edge1[1] = list_of_polygons[0].two.y - list_of_polygons[0].one.y;
		edge1[2] = list_of_polygons[0].two.z - list_of_polygons[0].one.z;

		edge2[0] = list_of_polygons[0].three.x - list_of_polygons[0].one.x;
		edge2[1] = list_of_polygons[0].three.y - list_of_polygons[0].one.y;
		edge2[2] = list_of_polygons[0].three.z - list_of_polygons[0].one.z;
		//////////////////////////////////////////////////////////////////
		int normal[3];
		//векторное произведение, получаем нормаль
		normal[0] = edge1[1] * edge2[2] - edge2[1] * edge1[2];
		normal[1] = edge1[2] * edge2[0] - edge2[2] * edge1[0];
		normal[2] = edge1[0] * edge2[1] - edge2[0] * edge1[1];
		//normal = find_normal(list_of_polygons[0]);
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
			float value = normal[0] * (list_of_polygons[0].one.x - main_polygon.one.x) + normal[1] * (list_of_polygons[0].one.y - main_polygon.one.y + normal[2] * (list_of_polygons[0].one.z - 1));
			int result = 0;
			if (value < 0)
			{
				result--;
			}
			else if (value > 0)
			{
				result++;
			}
			value = normal[0] * (list_of_polygons[0].two.x - main_polygon.one.x) + normal[1] * (list_of_polygons[0].two.y - main_polygon.one.y + normal[2] * (list_of_polygons[0].two.z - 1));
			if (value < 0)
			{
				result--;
			}
			else if (value > 0)
			{
				result++;
			}

			//int result = check_position(normal, main_polygon, list_of_polygons[0]);
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

	node* cmp(node* root, vector<polygon> list_of_polygons, node* prnt)
	{
		if (prnt != nullptr)
		{
			root->id_of_polygon = list_of_polygons[0].id_num;
			root->parent = prnt;
		}

		vector<polygon> front_list;
		vector<polygon> back_list;
		//составляем backlist и frontlist. после чего можно сделать рекурсивный вызов конструктора
		//с передачей корня и front/back листов
		ret_poly vec = classify_polygon(list_of_polygons, front_list, back_list);
		front_list = vec.front;
		back_list = vec.back;

		if (front_list.capacity() != 0)
		{
			node right(front_list[0].id_num);
			root->front = &right;
			cmp(root->front, front_list, root);

		}

		if (back_list.capacity() != 0)
		{
			node left(back_list[0].id_num);
			root->back = &left;
			cmp(root->back, back_list, root);

		}

		return root;
	}

	int cprnt(node* root)
	{
		if (root != NULL && root->back != NULL)
		{
			cprnt(root->back);
		}
		//cout<<root->id_of_polygon<<endl;
		if (root != NULL && root->front != NULL)
		{
			cprnt(root->front);
		}
		return 0;
	}

	BSP_CMP(node* root, vector<polygon> list_of_polygons)
	{
		root = cmp(root, list_of_polygons, NULL);
		//cprnt(root);

	}


};
int main()
{
	//создаём список полигонов;
	vector<polygon> list_of_polygons;

	//создаём несколько полигонов и заполняем их рандомными значениями
	//полигоны есть! осталось пропустить их через bsp комплилятор и построить bsp дерево.
	list_of_polygons = random::rand_input(list_of_polygons);
	/*
	list_of_polygons.resize(3);
	list_of_polygons[0].id_num = 0;
	list_of_polygons[0].one.x = 6;
	list_of_polygons[0].one.y = 8;
	list_of_polygons[0].one.z = 1;
	list_of_polygons[0].two.x = 18;
	list_of_polygons[0].two.y = 16;
	list_of_polygons[0].two.z = 1;
	list_of_polygons[0].three.x = 6;
	list_of_polygons[0].three.y = 8;
	list_of_polygons[0].three.z = 0;

	list_of_polygons[1].id_num = 1;
	list_of_polygons[1].one.x = 20;
	list_of_polygons[1].one.y = 8;
	list_of_polygons[1].one.z = 1;
	list_of_polygons[1].two.x = 24;
	list_of_polygons[1].two.y = 5;
	list_of_polygons[1].two.z = 1;
	list_of_polygons[1].three.x = 20;
	list_of_polygons[1].three.y = 8;
	list_of_polygons[1].three.z = 0;

	list_of_polygons[2].id_num = 2;
	list_of_polygons[2].one.x = 2;
	list_of_polygons[2].one.y = 11;
	list_of_polygons[2].one.z = 1;
	list_of_polygons[2].two.x = 2;
	list_of_polygons[2].two.y = 18;
	list_of_polygons[2].two.z = 1;
	list_of_polygons[2].three.x = 2;
	list_of_polygons[2].three.y = 11;
	list_of_polygons[2].three.z = 0;
	node root(0);
	*/
    node root(0);
	unsigned int start_time = clock();
	BSP_CMP Tree(&root, list_of_polygons);
	unsigned int end_time = clock();
	cout<<"run_time:"<<((end_time-start_time)/1000)<<endl;
	return 0;
}