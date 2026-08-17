#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ---------- 并查集的数据结构 ----------
typedef struct UnionFind{
    vector<int> parent; 
    vector<int> rank; 
    int count;
}UF;

// ---------- 1) 初始化 ----------
void UF_init(UF &uf, int n){
    uf.parent.resize(n);
    uf.rank.resize(n, 1);
    uf.count = n;
    for(int i = 0; i < n; i++){
        uf.parent[i] = i; 
    }
}

// ---------- 2) 查找 ----------
int UF_find(UF &uf, int x){
    if(uf.parent[x] != x){ 
        uf.parent[x] = UF_find(uf, uf.parent[x]);
    }
    return uf.parent[x];
}

// ---------- 判断 a、b 是否同集合 ----------
bool UF_isConnected(UF &uf, int a, int b){
    return UF_find(uf, a) == UF_find(uf, b);
}

// ---------- 3) 合并（带按秩合并）----------
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

// ---------- 4) 获取集合个数 ----------
int UF_getCount(UF &uf){
    return uf.count;
}

// ---------- 5) 打印所有集合 ----------
void UF_printSets(UF &uf){
    if(uf.parent.empty()){
        cout << "（并查集还没初始化！）" << endl;
        return;
    }
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

// ---------- 辅助：检查下标 x 是否合法 ----------
bool UF_checkIndex(UF &uf, int x){
    return x >= 0 && x < (int)uf.parent.size();
}

int main(){
    cout << "========== 并查集的基本操作（选项 1~5 共用同一个并查集）==========" << endl;
    cout << "1. 初始化并查集" << endl;
    cout << "2. 合并两个元素所在的集合" << endl;
    cout << "3. 查找元素所在的集合" << endl;
    cout << "4. 获取集合的个数" << endl;
    cout << "5. 打印所有集合" << endl;
    cout << "======================== 并查集的应用 =========================" << endl;
    cout << "6. 统计省份数量（统计连通块个数）" << endl;
    cout << "7. 判断图里有没有环（无向图判断环）" << endl;
    cout << "8. Kruskal 最小生成树" << endl;
    cout << "0. 退出" << endl;

    UF uf;
    bool initialized = false; // 标记 uf 是否已经初始化过

    int a = -1;
    cin >> a;
    while(a != 0){
        switch(a){
            case 1: {
                cout << "请输入并查集的大小 n：";
                int n;
                cin >> n;
                if(n <= 0){
                    cout << "n 必须大于 0，请重新输入" << endl;
                    break;
                }
                UF_init(uf, n);
                initialized = true;
                cout << "并查集初始化完成！" << n << " 个元素目前各自成团：" << endl;
                UF_printSets(uf);
                break;
            }
            case 2: {
                if(!initialized){ cout << "请先选 1 初始化并查集！" << endl; break; }
                cout << "请输入要合并的两个元素 x 和 y：";
                int x, y;
                cin >> x >> y;
                if(!UF_checkIndex(uf, x) || !UF_checkIndex(uf, y)){
                    cout << "下标越界！x、y 要在 0 ~ " << (int)uf.parent.size()-1 << " 之间" << endl;
                    break;
                }
                UF_unionSets(uf, x, y);
                cout << "已合并 " << x << " 和 " << y << "，现在的集合如下：" << endl;
                UF_printSets(uf);
                break;
            }
            case 3: {  // 查找
                if(!initialized){ cout << "请先选 1 初始化并查集！" << endl; break; }
                cout << "请输入要查找的元素 b：";
                int b;
                cin >> b;
                if(!UF_checkIndex(uf, b)){
                    cout << "下标越界！b 要在 0 ~ " << (int)uf.parent.size()-1 << " 之间" << endl;
                    break;
                }
                int root = UF_find(uf, b);
                cout << "元素 " << b << " 的根是：" << root << endl;
                cout << "它所在的集合是：{ ";
                for(int i = 0; i < (int)uf.parent.size(); i++){
                    if(UF_find(uf, i) == root){
                        cout << i << " ";
                    }
                }
                cout << "}" << endl;
                break;
            }
            case 4: {
                if(!initialized){ cout << "请先选 1 初始化并查集！" << endl; break; }
                cout << "当前集合的个数为：" << UF_getCount(uf) << endl;
                break;
            }
            case 5: {
                if(!initialized){ cout << "请先选 1 初始化并查集！" << endl; break; }
                cout << "当前所有集合如下：" << endl;
                UF_printSets(uf);
                break;
            }
            case 6: {
                cout << "请输入城市数量 n：";
                int n;
                cin >> n;
                if(n <= 0){ cout << "n 必须大于 0，请重新输入" << endl; break; }
                UF uf_app;
                UF_init(uf_app, n);
                cout << "请输入城市之间的连接关系（邻接矩阵，一行行输入）：" << endl;
                vector<vector<int>> graph(n, vector<int>(n));
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < n; j++){
                        cin >> graph[i][j];
                    }
                }
                for(int i = 0; i < n; i++){
                    for(int j = i + 1; j < n; j++){
                        if(graph[i][j] == 1){
                            UF_unionSets(uf_app, i, j);
                        }
                    }
                }
                cout << "这 " << n << " 个城市一共分成 " << UF_getCount(uf_app) << " 个省份" << endl;
                cout << "每个省份里都有哪些城市：" << endl;
                UF_printSets(uf_app);
                break;
            }
            case 7: {
                cout << "请输入图的顶点数 n：";
                int n;
                cin >> n;
                if(n <= 0){ cout << "n 必须大于 0，请重新输入" << endl; break; }
                cout << "请输入图的边数 m：";
                int m;
                cin >> m;
                UF uf_app;
                UF_init(uf_app, n);
                cout << "请输入每条边的两个端点 u 和 v：" << endl;
                bool hasCycle = false;
                for(int i = 0; i < m; i++){
                    int u, v;
                    while(true){
                        cin >> u >> v;
                        if(u >= 0 && u < n && v >= 0 && v < n) break;
                        cout << "顶点编号要在 0 ~ " << n-1 << " 之间，重新输入这条边：" << endl;
                    }
                    if(UF_isConnected(uf_app, u, v)){
                        hasCycle = true;
                        cout << "边 " << u << "-" << v << " 让图出现了环" << endl;
                    } else {
                        UF_unionSets(uf_app, u, v);
                    }
                }
                cout << (hasCycle ? "结论：图里有环" : "结论：图里没有环") << endl;
                break;
            }
            case 8: {
                cout << "请输入图的顶点数 n：";
                int n;
                cin >> n;
                if(n <= 0){ cout << "n 必须大于 0，请重新输入" << endl; break; }
                cout << "请输入图的边数 m：";
                int m;
                cin >> m;
                vector<vector<int>> edges(m, vector<int>(3));
                cout << "请输入每条边的起点、终点和权值：" << endl;
                for(int i = 0; i < m; i++){
                    while(true){
                        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
                        if(edges[i][0] >= 0 && edges[i][0] < n && edges[i][1] >= 0 && edges[i][1] < n) break;
                        cout << "顶点编号要在 0 ~ " << n-1 << " 之间，重新输入这条边：" << endl;
                    }
                }
                // 按权值从小到大排序（贪心：优先挑便宜的边）
                sort(edges.begin(), edges.end(), [](const vector<int> &a, const vector<int> &b){
                    return a[2] < b[2];
                });
                UF uf_app;
                UF_init(uf_app, n);
                int totalWeight = 0;
                cout << "Kruskal 最小生成树的边如下：" << endl;
                for(auto &e : edges){
                    if(!UF_isConnected(uf_app, e[0], e[1])){
                        UF_unionSets(uf_app, e[0], e[1]);
                        totalWeight += e[2];
                        cout << "边 " << e[0] << "-" << e[1] << " 权值 " << e[2] << endl;
                    }
                }
                cout << "最小生成树的总权值为：" << totalWeight << endl;
                break;
            }
            default:
                cout << "无效的选项，请重新输入。" << endl;
                break;
        }
        // 读取下一个选项；如果输入了非数字，清掉错误状态重新来
        if(!(cin >> a)){
            cin.clear();
            cin.ignore(10000, '\n');
            a = -1;
        }
    }
    cout << "程序已退出" << endl;
    return 0;
}

