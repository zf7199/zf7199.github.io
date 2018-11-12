#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>

using namespace std;

const int maxSize = 1000;
const int INF = 10e6;

const string filename = "/home/zf/CLionProjects/Dijkstra/testG1.txt";
const string filename2 = "/home/zf/CLionProjects/Dijkstra/testG2.txt";

typedef struct {
    int edges[maxSize][maxSize];
    int n, e;
}MGraph;

//将一个数组以向量的形式写入到文件中
void writeArray(vector<vector<int> > vec, int n, const string filename2){

    ofstream outfile;
    outfile.open(filename2);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            outfile << vec[i][j] << " ";
        }
        outfile << endl;
    }
    outfile.close();
}

//从文件中读取出数组
void ReadGraph(vector<vector<int> >&vec, const string &filename, int n){

    ifstream infile;
    infile.open(filename);
    int num = 1;

    for (int i = 0; i < n; ++i) {
        vec.resize(n);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            infile >> num;
            vec[i][j] = num;
        }
    }

    infile.close();
}

//生成一个n*n 的二维数组
vector<vector<int> > generateRandomArray(int n){

    vector<vector<int> > vec(n, vector<int>(n, 0));

    srand(time(NULL));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(i == j)
                vec[i][j] = 0;
            else
                vec[i][j] = rand() % 200;
        }
    }

    //检测权值
    //如果权值大于100，设置权值权值为INF，如果权值小于5，设置权值为0
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if(vec[i][j] < 5)
                vec[i][j] = 0;
            else if(vec[i][j] > 100)
                vec[i][j] = INF;
        }
    }

    return vec;
}

//创建一个图
MGraph *createMGraph(vector<vector<int> > vec, int n){
    MGraph *g = (MGraph*)malloc(sizeof(MGraph));
    g->n = n;
    for (int i = 0; i < g->n; ++i) {
        for (int j = 0; j < g->n; ++j) {
            g->edges[i][j] = vec[i][j];
        }
    }
    cout << "success!" << endl;
    return  g;
}

void Dijkstra(MGraph *graph, int v, int* dist, int* path){

    //set[i]标记顶点是否被访问
    bool set[graph->n];
    int min;
    int u = -1;

    //初始化
    for (int i = 0; i < graph->n; ++i) {
        //dist[i]存储了从源点v到i的最短路径长度
        dist[i] = graph->edges[v][i];
        set[i] = false;

        //path[i]存储了从源点v到i的最短路径上的前一个顶点
        //如果从v到i没有路径，则path[i] = -1
        if(graph->edges[v][i] < INF)
            path[i] = v;
        else
            path[i] = -1;
    }

    //访问顶点v
    set[v] = true;
    path[v] = -1;

    //关键操作开始
    for (int i = 0; i < graph->n - 1; ++i) {
        min = INF;
        //每次从剩余顶点中选出一个顶点，通往这个顶点的路径在所有通往生于顶点的路径中是长度最长的
        for (int j = 0; j < graph->n; ++j) {
            if(set[j] == false && dist[j] < min){
                u = j;
                min = dist[j];
            }
        }
        //将选出的顶点并入最短路径中
        set[u] = true;
        //以刚并入的顶点作为中间点，对所有通往剩余顶点的路径进行检测
        for (int k = 0; k < graph->n; ++k) {
            //判断顶点u的加入是否会出现通往顶点j的更短路径，如果出现，则改变了路径长度
            if(set[k] == false && dist[u] + graph->edges[u][k] < dist[k]){
                dist[k] = dist[u] + graph->edges[u][k];
                path[k] = u;
            }
        }
    }
}

void Floyd(MGraph *graph, vector<vector<int> > &A, vector<vector<int> > &B){

    //初始化A， B数组
    for (int i = 0; i < graph->n; ++i) {
        for (int j = 0; j < graph->n; ++j) {
            A[i][j] = graph->edges[i][j];
        }
    }

    for (int i = 0; i < graph->n; ++i) {
        for (int j = 0; j < graph->n; ++j) {
            B[i][j] = -1;
        }
    }

    //Floyd算法
    for (int i = 0; i < graph->n; ++i) {
        for (int j = 0; j < graph->n; ++j) {
            for (int k = 0; k < graph->n; ++k) {
                if (A[i][j] > A[i][k] + A[k][j]){
                    A[i][j] = A[i][k] + A[k][j];
                    B[i][j] = k;
                }
            }
        }
    }
}


int main() {

    vector<vector<int >> vec(maxSize, vector<int>(maxSize, 0));
    vector<vector<int >> vec2(7, vector<int>(7, 0));

    //生成maxSize * maxSize大小的数组并保存到vec中
    //vec = generateRandomArray(maxSize);

    //将vec 中的数组写到文件filename2中
    //writeArray(vec, maxSize, filename2);

    //将图从文件中读取到vec 中
    //ReadGraph(vec, filename2, maxSize);

    int a[7][7] = {
            {0, 4, 6, 6, INF, INF, INF},
            {INF, 0, 1, INF, 7, INF, INF},
            {INF, INF, 0, INF, 6, 4, INF},
            {INF, INF, 2, 0, INF, 5, INF},
            {INF, INF, INF, INF, 0, INF, 6},
            {INF, INF, INF, INF, 1, 0, 8},
            {INF, INF, INF, INF, INF, INF, 0},
    };

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            vec2[i][j] = a[i][j];
        }
    }

    MGraph *graph = createMGraph(vec2, 7);

    cout << graph->n << endl;

    int dist[maxSize] = { -1 };
    int path[maxSize] = { -1 };

    clock_t startTime, endTime;

    startTime = clock();

    //从0到剩余顶点的最短路径
    Dijkstra(graph, 0, dist, path);

    endTime = clock();

    for (int i = 0; i < 7; ++i) {
        cout << dist[i] << " ";
    }
    cout << endl << endl;
    for (int i = 0; i < 7; ++i) {
        cout << path[i] << " ";
    }
    cout << endl;

    cout << "Totle time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s!" << endl << endl;

    //测试Floyd
    vector<vector<int >> A(7, vector<int>(7, -1));
    vector<vector<int >> B(7, vector<int>(7, -1));

    Floyd(graph, A, B);

    for (int i = 0; i < graph->n; ++i) {
        for (int j = 0; j < graph->n; ++j) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
    for (int i = 0; i < graph->n; ++i) {
        for (int j = 0; j < graph->n; ++j) {
            cout << B[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
