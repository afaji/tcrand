#pragma once
#include <cstring>
#include "random.hpp"
#include "../object.hpp"

using namespace std;

namespace tcrand {
		
		void find_articulation_point(int pos, int par, vector<int> path[], bool visited[],int& time, int vis_time[], int low[], bool ap[]){
			if (visited[pos])
				return;
			visited[pos] = true;
			time++;
			vis_time[pos] = time;
			low[pos] = time;
			int M = path[pos].size();
			int child = 0;
			for (int i=0;i<M;i++){
				int u = path[pos][i];
				if (!visited[u]) {
					child++;
					find_articulation_point(u, pos, path, visited, time, vis_time, low, ap);

					low[pos] = min(low[pos], low[u]);
					if (par == -1 && child > 1)
						ap[pos] = true;
					else if (par != -1 && low[u] >= vis_time[pos])
						ap[pos] = true;
				}
				else if (u != par)
					low[pos] = min(low[pos], vis_time[u]);
			}
		}
		
		vector<int> articulation_points(Graph &g){
			int N = g.node_count();
			vector<int> from = g.edges().first;
			vector<int> to = g.edges().second;
			int E = from.size();
			int base = from[0];
			for (int i=0;i<E;i++){
				base = min(base, min(from[i], to[i]));
			}

			vector<int> path[N];
			for (int i=0;i<E;i++){
				path[ from[i] - base ].push_back(to[i] - base);
				path[ to[i] - base ].push_back(from[i] - base);
			}

			bool visited[N];
			int vis_time[N];
			int low[N];
			memset(visited, 0, sizeof visited);
			memset(low, 0, sizeof low);
			memset(vis_time, 0, sizeof vis_time);

			bool is_ap[N];
			memset(is_ap, 0 , sizeof is_ap);
			int time = 0;
			for (int i=0;i<N;i++){
				if (!visited[i])
					find_articulation_point(i, -1,path , visited , time , vis_time, low,is_ap);
			}

			vector <int> ap;
			for (int i=0;i<N;i++)
				if (is_ap[i])
					ap.push_back(i + base);

			return ap;
		}

}

/*
Let disc_time[v] = -1 and explored[v] = false forall v
dfscounter = 0

DFS(v):
  explored[v] = true
  disc_time[v] = low[v] = ++dfscounter
  foreach edge (v,x):
    if !explored[x]:
      DFS(x)
      low[v] = min(low[v], low[x])
      if low[x] >= disc_time[v]:
        print “v is articulation point separating x”
    elif x is not v’s parent:
low[v] = min(low[v], disc_time[x])


}*/