#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef struct UnionFind{
    vector<int> parent;
    vector<int> rank;
    int count;
}UF;
//设置并查集的初始化函数

void UF_init(UF &uf, int n){
    uf.parent.resize(n);
    uf.rank.resize(n, 0);
    uf.count = n;
    for(int i = 0; i < n; i++){
        uf.parent[i] = i;
    }
}
//并查集的构造函数 UF_init

int UF_find(UF &uf, int x){
    if(uf.parent[x] != x){
        uf.parent[x] = UF_find(uf, uf.parent[x]);
    }
    return uf.parent[x];
}
//并查集的查找函数 UF_find

bool UF_isConnected(UF &uf, int a, int b){
    return UF_find(uf, a) == UF_find(uf, b);
}
//并查集的判断函数 UF_isConnected

void UF_unionSets(UF &uf, int x, int y){
    int rx = UF_find(uf, x);
    int ry = UF_find(uf, y);
    if(rx == ry) return;
    if(uf.rank[rx] < uf.rank[ry]){
        uf.parent[rx] = ry;
    }
    else if(uf.rank[rx] > uf.rank[ry]){
        uf.parent[ry] = rx;
    }
    else{
        uf.parent[ry] = rx;
        uf.rank[rx]++;
    }
    uf.count--;
}
//并查集的合并函数 UF_unionSets

int UF_getCount(UF &uf){
    return uf.count;
}
//并查集的获取集合个数函数 UF_getCount

void UF_printSets(UF &uf){
    vector<vector<int>> groups(uf.parent.size());
    for(int i = 0; i < (int)uf.parent.size(); i++){
        int root = UF_find(uf, i);
        groups[root].push_back(i);
    }
    for(int r = 0; r < (int)groups.size(); r++){
        if(groups[r].empty()) continue;
        cout << "集合{根=" << r << "}: ";
        for(int e : groups[r]){
            cout << e << " ";
        }
        cout << endl;
    }
}
//并查集的打印函数 UF_printSets

int main(){
    cout << "========== 并查集的基本操作 ==========" << endl;
    cout << "1. 初始化并查集" << endl;
    cout << "2. 合并两个元素所在的集合" << endl;
    cout << "3. 查找元素所在的集合" << endl;
    cout << "4. 获取集合的个数" << endl;
    cout << "5. 打印所有集合" << endl;
    cout << "============ 并查集的应用 ===========" << endl;
    cout << "6. 统计省份数量（统计连通块个数）" << endl;
    cout << "7. 判断图里有没有环（无向图判断环）" << endl;
    cout << "8. Kruskal 最小生成树" << endl;
    cout << "0. 退出" << endl;

    int a;
    cin >> a;
    while(a != 0){
        switch(a){
            case 1:
                cout << "请输入并查集的大小 n：" << endl;
                int n;
                cin >> n;
                UF uf;
                UF_init(uf, n);
                cout << "并查集初始化完成" << endl;
                break;
            case 2:
                cout << "请输入要合并的两个元素 x 和 y：" << endl;
                int x, y;
                cin >> x >> y;
                UF_unionSets(uf, x, y);
                cout << "已合并元素 " << x << " 和 " << y << endl;
                break;
            case 3:
                cout << "请输入要查找的元素 b：" << endl;
                int b;
                cin >> b;
                cout << "元素 " << b << " 的根是：" << UF_find(uf, b) << endl;
                break;
            case 4:
                cout << "当前集合的个数为：" << UF_getCount(uf) << endl;
                break;
            case 5:
                cout << "当前所有集合如下：" << endl;
                UF_printSets(uf);
                break;
            case 6:
                cout << "请输入城市数量 n：" << endl;
                int n;
                cin >> n;
                UF uf;
                UF_init(uf, n);
                cout << "并查集初始化完成"<< endl;
                cout << "请输入城市之间的连接关系（邻接矩阵）：" << endl;
                vector<vector<int>> graph(n, vector<int>(n));
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < n; j++){
                        cin >> graph[i][j];
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = i + 1; j < n; j++){
                        if(graph[i][j] == 1){
                            UF_unionSets(uf, i, j);
                        }
                    }
                }
                cout << "这 " << n << " 个城市一共分成 " << UF_getCount(uf) << " 个省份" << endl;
                cout << "每个省份里都有哪些城市：" << endl;
                UF_printSets(uf);
                break;
            case 7:
                cout << "请输入图的边数 m：" << endl; 
                int m;
                cin >> m;
                UF uf;
                UF_init(uf, m);
                cout << "请输入每条边的两个端点 u 和 v：" << endl;
                bool hasCycle = false;
                for(int i = 0; i < m; i++){
                    int u, v;
                    cin >> u >> v;
                    if(UF_isConnected(uf, u, v)){
                        hasCycle = true;
                        cout << "边 " << u << "-" << v << " 让图出现了环" << endl;
                    } else {
                        UF_unionSets(uf, u, v);
                    }
                }
                cout << (hasCycle ? "结论：图里有环" : "结论：图里没有环") << endl;
                break;
            case 8:
                cout << "请输入图的边数 m：" << endl;
                int m;
                cin >> m;
                vector<vector<int>> edges(m, vector<int>(3));
                cout << "请输入每条边的起点、终点和权值：" << endl;
                for(int i = 0; i < m; i++){
                    cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
                }
                sort(edges.begin(), edges.end(), [](const vector<int> &a, const vector<int> &b){
                    return a[2] < b[2];
                });
                UF uf;
                UF_init(uf, m);
                int totalWeight = 0;
                cout << "Kruskal 最小生成树的边如下：" << endl;
                for(auto &e : edges){
                    if(!UF_isConnected(uf, e[0], e[1])){
                        UF_unionSets(uf, e[0], e[1]);
                        totalWeight += e[2];
                        cout << "边 " << e[0] << "-" << e[1] << " 权值 " << e[2] << endl;
                    }
                }
                cout << "最小生成树的总权值为：" << totalWeight << endl;
                break;
            default:
                cout << "无效的选项，请重新输入。" << endl;
        }
        cin >> a;
    }
}