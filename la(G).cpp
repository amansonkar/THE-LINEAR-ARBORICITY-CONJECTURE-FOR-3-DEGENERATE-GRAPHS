/*
 * " A Linear time algorithm for Linear Arboricity in 3-degenerate graph "
 *
 * author: aman sonkar
 */
#include <bits/stdc++.h>
using namespace std;
/* initialize the required data structures */
struct vNode
{
  struct vNode *prev, *next;
  int adj_v;
  struct eNode *edge;
  bool special;
  struct segmentNode *segment;
};
struct eNode
{
  struct eNode *prev, *next;
  int u, v, color;
  struct vNode *Nu, *Nv;
};
struct Pivot
{
  struct Pivot *prev, *next;
  int v;
};
struct clrNode
{
  struct clrNode *prev, *next;
  int color;
  struct vNode *Nu;
};
struct segmentNode
{
  struct vNode *Nu, *Nv;
  int u, v;
};
int K = 2, tN, N;
vector<vector<int>> adj;
vector<int> d, dG, ddG;
vector<pair<struct vNode *, struct vNode *>> adjList;
pair<struct eNode *, struct eNode *> Edges;
pair<struct Pivot *, struct Pivot *> Pivots;
vector<struct Pivot *> VtoPivot;
vector<pair<struct clrNode *, struct clrNode *>> Once, Miss;
vector<vector<struct clrNode *>> Ptrs;
vector<set<int>> Colors;

void Show_Edges()
{
  struct eNode *head_edge_node = Edges.first;
  while (head_edge_node != NULL)
  {
    if (head_edge_node->u >= N || head_edge_node->v >= N)
      cout << "dummy ";
    cout << "Edge (" << head_edge_node->u + 1 << "," << head_edge_node->v + 1 << ") colored by Color=" << head_edge_node->color << "\n";
    head_edge_node = head_edge_node->next;
  }
}

void Show_Pivots()
{
  struct Pivot *head_pivot_node = Pivots.first;
  while (head_pivot_node != NULL)
  {
    cout << "Pivot : " << head_pivot_node->v + 1 << ",\n";
    head_pivot_node = head_pivot_node->next;
  }
}

void add_Edge(int u, int v)
{
  d[u]++;
  d[v]++;
  dG[u]++;
  dG[v]++;
  struct vNode *new_u_node = (struct vNode *)malloc(sizeof(struct vNode));
  struct vNode *new_v_node = (struct vNode *)malloc(sizeof(struct vNode));
  struct eNode *new_e_node = (struct eNode *)malloc(sizeof(struct eNode));
  new_u_node->prev = adjList[u].second;
  new_u_node->adj_v = v;
  new_u_node->edge = new_e_node;
  new_u_node->segment = NULL;
  new_u_node->special = false;
  new_u_node->next = NULL;
  if (adjList[u].first == NULL)
    adjList[u].second = adjList[u].first = new_u_node;
  else
    adjList[u].second = adjList[u].second->next = new_u_node;
  new_v_node->prev = adjList[v].second;
  new_v_node->adj_v = u;
  new_v_node->edge = new_e_node;
  new_v_node->segment = NULL;
  new_v_node->special = false;
  new_v_node->next = NULL;
  if (adjList[v].first == NULL)
    adjList[v].second = adjList[v].first = new_v_node;
  else
    adjList[v].second = adjList[v].second->next = new_v_node;
  new_e_node->prev = Edges.second;
  new_e_node->u = u;
  new_e_node->v = v;
  new_e_node->color = -1;
  new_e_node->Nu = new_u_node;
  new_e_node->Nv = new_v_node;
  new_e_node->next = NULL;
  if (Edges.first == NULL)
    Edges.second = Edges.first = new_e_node;
  else
    Edges.second = Edges.second->next = new_e_node;
}

void Encoding_the_graph(int N)
{
  ddG = dG = d = vector<int>(N, 0);
  adjList.resize(N, {NULL, NULL});
  Edges = {NULL, NULL};
  for (int u = 0; u < N; u++)
  {
    // sort(adj[u].begin(), adj[u].end());
    int sz = adj[u].size();
    K = max(K, (sz + 2) / 2);
    for (int v : adj[u])
    {
      if (u < v)
        add_Edge(u, v);
    }
  }
}

void add_Miss_node(int u, int clr)
{
  struct clrNode *new_miss_node = (struct clrNode *)malloc(sizeof(struct clrNode));
  new_miss_node->prev = Miss[u].second;
  new_miss_node->color = clr;
  new_miss_node->next = NULL;
  if (Miss[u].first == NULL)
    Miss[u].second = Miss[u].first = new_miss_node;
  else
    Miss[u].second = Miss[u].second->next = new_miss_node;
  Ptrs[u].push_back(new_miss_node);
}

void Encoding_the_coloring(int N)
{
  Once.resize(N, {NULL, NULL});
  Miss.resize(N, {NULL, NULL});
  Colors.resize(N, {});
  Ptrs.resize(N);
  for (int u = 0; u < N; u++)
  {
    for (int i = 0; i < min(d[u] + 2, K); i++)
    {
      add_Miss_node(u, i);
    }
  }
}

struct Pivot *add_Pivot(int node)
{
  struct Pivot *new_pivot = (struct Pivot *)malloc(sizeof(struct Pivot));
  new_pivot->prev = Pivots.second;
  new_pivot->v = node;
  new_pivot->next = NULL;
  if (Pivots.first == NULL)
    Pivots.second = Pivots.first = new_pivot;
  else
    Pivots.second = Pivots.second->next = new_pivot;
  return new_pivot;
}

void The_Pivots_list(int N)
{
  Pivots = {NULL, NULL};
  VtoPivot.resize(N, NULL);
  for (int u = 0; u < N; u++)
  {
    for (int v : adj[u])
    {
      if (dG[v] <= 3)
        ddG[u]++;
    }
  }
  for (int u = 0; u < N; u++)
  {
    if (ddG[u] > 0 && (dG[u] - ddG[u] <= 3) && VtoPivot[u] == NULL)
      VtoPivot[u] = add_Pivot(u);
  }
}

void Preprocessing(int N)
{
  Encoding_the_graph(N);
  Encoding_the_coloring(N);
  The_Pivots_list(N);
}

void remove_Pivot(int node)
{
  if (VtoPivot[node] == NULL)
    return;
  struct Pivot *temp_pivot = VtoPivot[node];
  VtoPivot[node] = NULL;
  if (temp_pivot == Pivots.first)
    Pivots.first = temp_pivot->next;
  if (temp_pivot == Pivots.second)
    Pivots.second = temp_pivot->prev;
  if (temp_pivot->prev != NULL)
    temp_pivot->prev->next = temp_pivot->next;
  if (temp_pivot->next != NULL)
    temp_pivot->next->prev = temp_pivot->prev;
  free(temp_pivot);
}

void update_Pivot(struct eNode *edge)
{
  int u = edge->u, v = edge->v;
  if (dG[u] < 3)
    ddG[v]--;
  if (dG[v] < 3)
    ddG[u]--;
  if (ddG[u] == 0)
    remove_Pivot(u);
  if (ddG[v] == 0)
    remove_Pivot(v);
  if (dG[v] == 3)
  {
    struct vNode *head_node_v = adjList[v].first;
    while (head_node_v != NULL)
    {
      int w = head_node_v->adj_v;
      ddG[w]++;
      if (ddG[w] > 0 && dG[w] - ddG[w] <= 3 && VtoPivot[w] == NULL)
        VtoPivot[w] = add_Pivot(w);
      head_node_v = head_node_v->next;
    }
  }
  if (dG[u] == 3)
  {
    struct vNode *head_node_u = adjList[u].first;
    while (head_node_u != NULL)
    {
      int w = head_node_u->adj_v;
      ddG[w]++;
      if (ddG[w] > 0 && dG[w] - ddG[w] <= 3 && VtoPivot[w] == NULL)
        VtoPivot[w] = add_Pivot(w);
      head_node_u = head_node_u->next;
    }
  }
}

void remove_Edge(struct eNode *edge)
{
  dG[edge->u]--;
  dG[edge->v]--;
  struct vNode *adjList_u = edge->Nu;
  if (adjList_u == adjList[edge->u].first)
    adjList[edge->u].first = adjList_u->next;
  if (adjList_u == adjList[edge->u].second)
    adjList[edge->u].second = adjList_u->prev;
  if (adjList_u->prev != NULL)
    adjList_u->prev->next = adjList_u->next;
  if (adjList_u->next != NULL)
    adjList_u->next->prev = adjList_u->prev;
  struct vNode *adjList_v = edge->Nv;
  if (adjList_v == adjList[edge->v].first)
    adjList[edge->v].first = adjList_v->next;
  if (adjList_v == adjList[edge->v].second)
    adjList[edge->v].second = adjList_v->prev;
  if (adjList_v->prev != NULL)
    adjList_v->prev->next = adjList_v->next;
  if (adjList_v->next != NULL)
    adjList_v->next->prev = adjList_v->prev;
  if (edge == Edges.first)
    Edges.first = edge->next;
  if (edge == Edges.second)
    Edges.second = edge->prev;
  if (edge->prev != NULL)
    edge->prev->next = edge->next;
  if (edge->next != NULL)
    edge->next->prev = edge->prev;
  update_Pivot(edge);
}

void add_removed_Edge(struct eNode *edge)
{
  edge->Nu->prev = adjList[edge->u].second;
  edge->Nu->next = NULL;
  if (adjList[edge->u].first == NULL)
    adjList[edge->u].second = adjList[edge->u].first = edge->Nu;
  else
    adjList[edge->u].second = adjList[edge->u].second->next = edge->Nu;
  edge->Nv->prev = adjList[edge->v].second;
  edge->Nv->next = NULL;
  if (adjList[edge->v].first == NULL)
    adjList[edge->v].second = adjList[edge->v].first = edge->Nv;
  else
    adjList[edge->v].second = adjList[edge->v].second->next = edge->Nv;
  edge->prev = Edges.second;
  edge->next = NULL;
  if (Edges.first == NULL)
    Edges.second = Edges.first = edge;
  else
    Edges.second = Edges.second->next = edge;
}

void add_Once_node(int clr, int u, struct eNode *edge)
{
  struct clrNode *new_Once_node = (struct clrNode *)malloc(sizeof(struct clrNode));
  new_Once_node->prev = Once[u].second;
  new_Once_node->color = clr;
  if (u == edge->u)
    new_Once_node->Nu = edge->Nu;
  else if (u == edge->v)
    new_Once_node->Nu = edge->Nv;
  else
    cout << "Something Went Wrong...\n";
  new_Once_node->next = NULL;
  if (Once[u].first == NULL)
    Once[u].second = Once[u].first = new_Once_node;
  else
    Once[u].second = Once[u].second->next = new_Once_node;
}

void remove_Miss_node(int u, int clr)
{
  struct clrNode *temp_miss_node = Ptrs[u][clr];
  Ptrs[u][clr] = NULL;
  if (temp_miss_node == Miss[u].first)
    Miss[u].first = temp_miss_node->next;
  if (temp_miss_node == Miss[u].second)
    Miss[u].second = temp_miss_node->prev;
  if (temp_miss_node->prev != NULL)
    temp_miss_node->prev->next = temp_miss_node->next;
  if (temp_miss_node->next != NULL)
    temp_miss_node->next->prev = temp_miss_node->prev;
  free(temp_miss_node);
}

void remove_Once_node(int u, struct clrNode *once_node_u)
{
  if (Once[u].first == once_node_u)
    Once[u].first = once_node_u->next;
  if (Once[u].second == once_node_u)
    Once[u].second = once_node_u->prev;
  if (once_node_u->prev != NULL)
    once_node_u->prev->next = once_node_u->next;
  if (once_node_u->next != NULL)
    once_node_u->next->prev = once_node_u->prev;
  free(once_node_u);
}

struct segmentNode *fuse_segment(struct segmentNode *sigma1, struct segmentNode *sigma2, int u)
{
  int u1 = -1;
  struct vNode *N1 = NULL;
  if (sigma1->u == u)
  {
    sigma1->Nu->segment = NULL;
    u1 = sigma1->v;
    N1 = sigma1->Nv;
  }
  else
  {
    sigma1->Nv->segment = NULL;
    u1 = sigma1->u;
    N1 = sigma1->Nu;
  }
  int u2 = -1;
  struct vNode *N2 = NULL;
  if (sigma2->u == u)
  {
    sigma2->Nu->segment = NULL;
    u2 = sigma2->v;
    N2 = sigma2->Nv;
  }
  else
  {
    sigma2->Nv->segment = NULL;
    u2 = sigma2->u;
    N2 = sigma2->Nu;
  }
  struct segmentNode *sigma = (struct segmentNode *)malloc(sizeof(struct segmentNode));
  sigma->u = u1;
  sigma->Nu = N1;
  sigma->v = u2;
  sigma->Nv = N2;
  N1->segment = sigma;
  N2->segment = sigma;
  free(sigma1);
  free(sigma2);
  return sigma;
}

bool maximal_monochromatic_segment(int v, struct vNode *adj_vu, int u)
{
  if (adj_vu->segment == NULL)
    return false;
  struct segmentNode *sigma = adj_vu->segment;
  if (sigma->v == v && sigma->u == u)
    return true;
  else if (sigma->v == u && sigma->u == v)
    return true;
  return false;
}

struct segmentNode *create_new_segment(struct eNode *edge)
{
  struct segmentNode *sigma = (struct segmentNode *)malloc(sizeof(struct segmentNode));
  sigma->u = edge->u;
  sigma->Nu = edge->Nu;
  sigma->v = edge->v;
  sigma->Nv = edge->Nv;
  edge->Nu->segment = sigma;
  edge->Nv->segment = sigma;
  return sigma;
}

void coloring_with_Lemma5(struct eNode *edge, int color)
{
  if (edge->color >= 0)
    return;
  int u = edge->u, v = edge->v;
  if (u >= N || v >= N)
    return;
  //cout<<"coloring... : "<<u+1<<" ---- "<<v+1<<" = "<<color<<"\n";
  edge->color = color;
  struct segmentNode *sigma = create_new_segment(edge);
  Colors[u].insert(color);
  Colors[v].insert(color);
  struct clrNode *head_once_u = Once[u].first;
  bool flag = false;
  while (head_once_u != NULL)
  {
    if (head_once_u->color == color)
    {
      flag = true;
      break;
    }
    head_once_u = head_once_u->next;
  }
  if (!flag)
  {
    if (color < d[u] + 2)
      remove_Miss_node(u, color);
    add_Once_node(color, u, edge);
  }
  else
  {
    struct vNode *temp_Nu = head_once_u->Nu;
    remove_Once_node(u, head_once_u);
    if (edge->Nu->segment != NULL && temp_Nu->segment != NULL)
      sigma = fuse_segment(sigma, temp_Nu->segment, u);
  }
  struct clrNode *head_once_v = Once[v].first;
  flag = false;
  while (head_once_v != NULL)
  {
    if (head_once_v->color == color)
    {
      flag = true;
      break;
    }
    head_once_v = head_once_v->next;
  }
  if (!flag)
  {
    if (color < d[v] + 2)
      remove_Miss_node(v, color);
    add_Once_node(color, v, edge);
  }
  else
  {
    struct vNode *temp_Nu = head_once_v->Nu;
    remove_Once_node(v, head_once_v);
    if (edge->Nv->segment != NULL && temp_Nu->segment != NULL)
      fuse_segment(sigma, temp_Nu->segment, v);
  }
}

bool check_Colors(int u, int color)
{
  for (int i : Colors[u])
  {
    if (i == color)
      return true;
  }
  return false;
}

bool check_Twice(int vertex, int color)
{
  struct clrNode *head_once_node = Once[vertex].first;
  while (head_once_node != NULL)
  {
    if (head_once_node->color == color)
      return false;
    head_once_node = head_once_node->next;
  }
  for (int i : Colors[vertex])
  {
    if (i == color)
      return true;
  }
  return false;
}

bool check_O5(int v, int u, int clr, struct vNode *edge_vu)
{
  // Check Observation 5
  return (!check_Twice(u, clr) && !maximal_monochromatic_segment(v, edge_vu, u));
}

void add_dummy_vertex(int u)
{
  int x = tN++;
  adjList.push_back({NULL, NULL});
  d.push_back(0);
  dG.push_back(0);
  ddG.push_back(0);
  VtoPivot.push_back(NULL);
  add_Edge(u, x);
  ddG[u]++;
  if (ddG[u] > 0 && (dG[u] - ddG[u] <= 3) && VtoPivot[u] == NULL)
    VtoPivot[u] = add_Pivot(u);
  if (dG[u] <= 3)
    ddG[x]++;
  if (ddG[x] > 0 && (dG[x] - ddG[x] <= 3) && VtoPivot[x] == NULL)
    VtoPivot[x] = add_Pivot(x);
  Once.push_back({NULL, NULL});
  Miss.push_back({NULL, NULL});
  Ptrs.push_back({});
  Colors.push_back({});
  for (int i = 0; i < min(d[x] + 2, K); i++)
  {
    add_Miss_node(x, i);
  }
}

void COLOR()
{
  if (Pivots.first == NULL)
    return;
  int v = Pivots.first->v;
  // cout<<" v : "<<v+1<<"\n";
  int u = -1;
  struct eNode *edge_uv = NULL;
  struct vNode *head_node_v = adjList[v].first;
  while (head_node_v != NULL)
  {
    if (dG[head_node_v->adj_v] <= 3)
    {
      u = head_node_v->adj_v;
      edge_uv = head_node_v->edge;
      break;
    }
    head_node_v = head_node_v->next;
  }
  // cout<<" u : "<<u+1<<"\n";
  if (dG[v] < 2 * K - 1)
  {
    // cout << "1st condn : "<< v+1 << " " << u+1 << "\n";
    remove_Edge(edge_uv);
    COLOR();
    add_removed_Edge(edge_uv);
    if (Once[v].first != NULL && !check_Colors(u, Once[v].first->color))
      coloring_with_Lemma5(edge_uv, Once[v].first->color);
    else if (Once[v].first != NULL && Once[v].first->next != NULL && !check_Colors(u, Once[v].first->next->color))
      coloring_with_Lemma5(edge_uv, Once[v].first->next->color);
    else if (Once[v].first != NULL && Once[v].first->next != NULL && Once[v].first->next->next != NULL && !check_Colors(u, Once[v].first->next->next->color))
      coloring_with_Lemma5(edge_uv, Once[v].first->next->next->color);
    else if (Miss[v].first != NULL && !check_Twice(u, Miss[v].first->color))
      coloring_with_Lemma5(edge_uv, Miss[v].first->color);
    else if (Miss[v].first != NULL && Miss[v].first->next != NULL && !check_Colors(u, Miss[v].first->next->color))
      coloring_with_Lemma5(edge_uv, Miss[v].first->next->color);
    else if (Miss[v].first != NULL && Once[v].first != NULL && check_O5(v, u, Once[v].first->color, Once[v].first->Nu))
      coloring_with_Lemma5(edge_uv, Once[v].first->color);
    // cout<<"leaving 1st condn : "<<v+1<<" "<<u+1<<"\n";
  }
  else if (dG[v] == 2 * K - 1)
  {
    // cout << "2nd Condn\n";
    int w = -1;
    struct eNode *edge_vw = NULL;
    head_node_v = adjList[v].first;
    while (head_node_v != NULL)
    {
      if (dG[head_node_v->adj_v] <= 3 && head_node_v->adj_v != u)
      {
        w = head_node_v->adj_v;
        edge_vw = head_node_v->edge;
        break;
      }
      head_node_v = head_node_v->next;
    }
    // cout << " w : " << w + 1 << "\n";
    struct eNode *edge_uw = NULL;
    bool isExist_uw = false;
    struct vNode *head_node_u = adjList[u].first;
    while (head_node_u != NULL)
    {
      if (head_node_u->adj_v == w)
      {
        isExist_uw = true;
        edge_uw = head_node_u->edge;
        break;
      }
      head_node_u = head_node_u->next;
    }
    // cout << " is_Exist_uw : " << isExist_uw << "\n";
    bool isExist_CN_except_v = false;
    int z = -1;
    struct eNode *edge_uz = NULL, *edge_wz = NULL;
    head_node_u = adjList[u].first;
    while (head_node_u != NULL)
    {
      struct vNode *head_node_w = adjList[w].first;
      while (head_node_w != NULL)
      {
        if (head_node_u->adj_v == head_node_w->adj_v && head_node_w->adj_v != v)
        {
          z = head_node_w->adj_v;
          edge_uz = head_node_u->edge;
          edge_wz = head_node_w->edge;
          break;
        }
        head_node_w = head_node_w->next;
      }
      if (z >= 0)
      {
        isExist_CN_except_v = true;
        break;
      }
      head_node_u = head_node_u->next;
    }
    // cout << " is_Exist_CN_except_v : " << isExist_CN_except_v<<" ,\n z : "<<z+1<< "\n";
    if (isExist_uw)
    {
      if (dG[u] < 3)
        add_dummy_vertex(u);
      if (dG[w] < 3)
        add_dummy_vertex(w);
      int x = -1, y = -1;
      struct eNode *edge_ux = NULL, *edge_wy = NULL;
      head_node_u = adjList[u].first;
      while (head_node_u != NULL)
      {
        if (head_node_u->adj_v != v && head_node_u->adj_v != w)
        {
          x = head_node_u->adj_v;
          edge_ux = head_node_u->edge;
          break;
        }
        head_node_u = head_node_u->next;
      }
      // cout<<" x : "<<x+1<<"\n";
      struct vNode *head_node_w = adjList[w].first;
      while (head_node_w != NULL)
      {
        if (head_node_w->adj_v != v && head_node_w->adj_v != u)
        {
          y = head_node_w->adj_v;
          edge_wy = head_node_w->edge;
          break;
        }
        head_node_w = head_node_w->next;
      }
      // cout<<" y : "<<y+1<<"\n";
      remove_Edge(edge_uv);
      remove_Edge(edge_uw);
      remove_Edge(edge_ux);
      remove_Edge(edge_vw);
      remove_Edge(edge_wy);
      COLOR();
      add_removed_Edge(edge_uv);
      add_removed_Edge(edge_uw);
      add_removed_Edge(edge_ux);
      add_removed_Edge(edge_vw);
      add_removed_Edge(edge_wy);
      int i = -1;
      if (Miss[x].first != NULL)
        i = Miss[x].first->color;
      else if (Once[x].first != NULL)
        i = Once[x].first->color;
      coloring_with_Lemma5(edge_ux, i);
      if (Once[v].first != NULL && Once[v].first->next != NULL && Once[v].first->next->next != NULL)
      {
        int j = -1, k = -1;
        if (Once[v].first->color != i)
          j = Once[v].first->color;
        if (Once[v].first->next->color != i)
        {
          if (j == -1)
            j = Once[v].first->next->color;
          else
            k = Once[v].first->next->color;
        }
        if (Once[v].first->next->next->color != i && k == -1)
          k = Once[v].first->next->next->color;
        coloring_with_Lemma5(edge_uv, j);
        coloring_with_Lemma5(edge_uw, k);
        int l = -1;
        if (Once[v].first->color != j && Once[v].first->color != k)
          l = Once[v].first->color;
        else if (Once[v].first->next->color != j && Once[v].first->next->color != k)
          l = Once[v].first->next->color;
        else if (Once[v].first->next->next->color != j && Once[v].first->next->next->color != k)
          l = Once[v].first->next->next->color;
        coloring_with_Lemma5(edge_vw, l);
        if (Miss[y].first != NULL)
          coloring_with_Lemma5(edge_wy, Miss[y].first->color);
        else if (Once[y].first != NULL && Once[y].first->color != l)
          coloring_with_Lemma5(edge_wy, Once[y].first->color);
        else if (Once[y].first != NULL && Once[y].first->next != NULL && Once[y].first->next->color != l)
          coloring_with_Lemma5(edge_wy, Once[y].first->next->color);
      }
      else if (Miss[v].first != NULL && Once[v].first != NULL)
      {
        int j = Miss[v].first->color, k = Once[v].first->color;
        coloring_with_Lemma5(edge_uv, j);
        coloring_with_Lemma5(edge_vw, j);
        coloring_with_Lemma5(edge_uw, k);
        if (Miss[y].first != NULL)
          coloring_with_Lemma5(edge_wy, Miss[y].first->color);
        else if (Once[y].first != NULL && Once[y].first->color != i)
          coloring_with_Lemma5(edge_wy, Once[y].first->color);
        else if (Once[y].first != NULL && Once[y].first->next != NULL && Once[y].first->next->color != i)
          coloring_with_Lemma5(edge_wy, Once[y].first->next->color);
      }
    }
    else if (isExist_CN_except_v)
    {
      while (dG[u] < 3)
        add_dummy_vertex(u);
      while (dG[w] < 3)
        add_dummy_vertex(w);
      int x = -1, y = -1;
      struct eNode *edge_ux = NULL, *edge_wy = NULL;
      head_node_u = adjList[u].first;
      while (head_node_u != NULL)
      {
        if (head_node_u->adj_v != v && head_node_u->adj_v != z)
        {
          x = head_node_u->adj_v;
          edge_ux = head_node_u->edge;
          break;
        }
        head_node_u = head_node_u->next;
      }
      // cout<<" x : "<<x+1<<"\n";
      struct vNode *head_node_w = adjList[w].first;
      while (head_node_w != NULL)
      {
        if (head_node_w->adj_v != v && head_node_w->adj_v != z)
        {
          y = head_node_w->adj_v;
          edge_wy = head_node_w->edge;
          break;
        }
        head_node_w = head_node_w->next;
      }
      // cout<<" y : "<<y+1<<"\n";
      remove_Edge(edge_uv);
      remove_Edge(edge_uz);
      remove_Edge(edge_ux);
      remove_Edge(edge_vw);
      remove_Edge(edge_wz);
      remove_Edge(edge_wy);
      COLOR();
      add_removed_Edge(edge_uv);
      add_removed_Edge(edge_uz);
      add_removed_Edge(edge_ux);
      add_removed_Edge(edge_vw);
      add_removed_Edge(edge_wz);
      add_removed_Edge(edge_wy);
      if (Miss[v].first != NULL && (Miss[v].first->next != NULL || Once[v].first != NULL))
      {
        int i = Miss[v].first->color, j = -1;
        if (Miss[v].first->next != NULL)
          j = Miss[v].first->next->color;
        else if (Once[v].first != NULL)
          j = Once[v].first->color;
        if (Miss[z].first != NULL && (Miss[z].first->next != NULL || Once[z].first != NULL))
        {
          int p = Miss[z].first->color, q = -1, r = -1, l = -1;
          if (Miss[z].first->next != NULL)
            q = Miss[z].first->next->color;
          else if (Once[z].first != NULL)
            q = Once[z].first->color;
          if (i != p)
          {
            coloring_with_Lemma5(edge_uv, i);
            coloring_with_Lemma5(edge_vw, i);
            coloring_with_Lemma5(edge_wz, p);
            coloring_with_Lemma5(edge_uz, p);
            r = i;
            l = p;
          }
          else
          {
            coloring_with_Lemma5(edge_uv, i);
            coloring_with_Lemma5(edge_wz, i);
            coloring_with_Lemma5(edge_vw, j);
            coloring_with_Lemma5(edge_uz, q);
            r = j;
            l = q;
          }
          if (Miss[y].first != NULL && Miss[y].first->color != r)
            coloring_with_Lemma5(edge_wy, Miss[y].first->color);
          else if (Miss[y].first != NULL && Miss[y].first->next != NULL && Miss[y].first->next->color != r)
            coloring_with_Lemma5(edge_wy, Miss[y].first->next->color);
          else if (Once[y].first != NULL && Once[y].first->color != r)
            coloring_with_Lemma5(edge_wy, Once[y].first->color);
          else if (Once[y].first != NULL && Once[y].first->next != NULL && Once[y].first->next->color != r)
            coloring_with_Lemma5(edge_wy, Once[y].first->next->color);
          if (Miss[x].first != NULL && Miss[x].first->color != l)
            coloring_with_Lemma5(edge_ux, Miss[x].first->color);
          else if (Miss[x].first != NULL && Miss[x].first->next != NULL && Miss[x].first->next->color != l)
            coloring_with_Lemma5(edge_ux, Miss[x].first->next->color);
          else if (Once[x].first != NULL && Once[x].first->color != l)
            coloring_with_Lemma5(edge_ux, Once[x].first->color);
          else if (Once[x].first != NULL && Once[x].first->next != NULL && Once[x].first->next->color != l)
            coloring_with_Lemma5(edge_ux, Once[x].first->next->color);
        }
        else if (Once[z].first != NULL && Once[z].first->next != NULL && Once[z].first->next->next != NULL)
        {
          coloring_with_Lemma5(edge_uv, i);
          coloring_with_Lemma5(edge_vw, j);
          int t = -1, s = -1, l = -1, tl = -1;
          if (Miss[y].first != NULL)
            t = Miss[y].first->color;
          else if (Miss[y].first != NULL && Miss[y].first->next != NULL)
            t = Miss[y].first->next->color;
          else if (Once[y].first != NULL && Once[y].first->color != j)
            t = Once[y].first->color;
          else if (Once[y].first != NULL && Once[y].first->next != NULL && Once[y].first->next->color != j)
            t = Once[y].first->next->color;
          coloring_with_Lemma5(edge_wy, t);
          if (Once[x].first != NULL)
            s = Once[x].first->color;
          else if (Miss[x].first != NULL)
            s = Miss[x].first->color;
          coloring_with_Lemma5(edge_ux, s);
          if (Once[z].first->color != t && Once[z].first->color != j)
            l = Once[z].first->color;
          else if (Once[z].first->next->color != t && Once[z].first->next->color != j)
            l = Once[z].first->next->color;
          else if (Once[z].first->next->next->color != t && Once[z].first->next->next->color != j)
            l = Once[z].first->next->next->color;
          coloring_with_Lemma5(edge_wz, l);
          if (Once[z].first->color != s && Once[z].first->color != l)
            tl = Once[z].first->color;
          else if (Once[z].first->next->color != s && Once[z].first->next->color != l)
            tl = Once[z].first->next->color;
          else if (Once[z].first->next->next->color != s && Once[z].first->next->next->color != l)
            tl = Once[z].first->next->next->color;
          coloring_with_Lemma5(edge_uz, tl);
        }
      }
      else if (Once[v].first != NULL && Once[v].first->next != NULL && Once[v].first->next->next != NULL && Once[z].first != NULL && Once[z].first->next != NULL && Once[z].first->next->next != NULL)
      {
        int i = Once[v].first->color, j = Once[v].first->next->color, l = Once[v].first->next->next->color, t = -1, h = -1, s = -1, f = -1, tl = -1;
        coloring_with_Lemma5(edge_vw, j);
        if (Miss[y].first != NULL)
          t = Miss[y].first->color;
        else if (Miss[y].first != NULL && Miss[y].first->next != NULL)
          t = Miss[y].first->next->color;
        else if (Once[y].first != NULL && Once[y].first->color != j)
          t = Once[y].first->color;
        else if (Once[y].first != NULL && Once[y].first->next != NULL && Once[y].first->next->color != j)
          t = Once[y].first->next->color;
        coloring_with_Lemma5(edge_wy, t);
        if (i != t)
          h = i;
        else
          h = l;
        coloring_with_Lemma5(edge_uv, h);
        if (Miss[x].first != NULL)
          s = Miss[x].first->color;
        else if (Once[x].first != NULL && Once[x].first->color != h)
          s = Once[x].first->color;
        else if (Once[x].first->next != NULL && Once[x].first->next->color != h)
          s = Once[x].first->next->color;
        coloring_with_Lemma5(edge_ux, s);
        if (Once[z].first->color == h || (Once[z].first->color != t && Once[z].first->color != j))
          f = Once[z].first->color;
        else if (Once[z].first->next->color == h || (Once[z].first->next->color != t && Once[z].first->next->color != j))
          f = Once[z].first->next->color;
        else if (Once[z].first->next->next->color == h || (Once[z].first->next->next->color != t && Once[z].first->next->next->color != j))
          f = Once[z].first->next->next->color;
        coloring_with_Lemma5(edge_wz, f);
        if (Once[z].first->color != f && Once[z].first->color != s)
          tl = Once[z].first->color;
        else if (Once[z].first->next->color != f && Once[z].first->next->color != s)
          tl = Once[z].first->next->color;
        else if (Once[z].first->next->next->color != f && Once[z].first->next->next->color != s)
          tl = Once[z].first->next->next->color;
        coloring_with_Lemma5(edge_uz, tl);
      }
    }
    else if (!isExist_uw && !isExist_CN_except_v)
    {
      while (dG[w] < 3)
        add_dummy_vertex(w);
      int x = -1, y = -1;
      remove_Edge(edge_uv);
      remove_Edge(edge_vw);
      struct eNode *edge_wx = adjList[w].first->edge;
      x = adjList[w].first->adj_v;
      remove_Edge(edge_wx);
      // identification
      struct vNode *identified_node_wy = adjList[w].first;
      struct eNode *identified_edge_wy = identified_node_wy->edge;
      y = identified_node_wy->adj_v;
      adjList[w] = {NULL, NULL};
      dG[w] = 0;
      dG[u]++;
      struct vNode *new_u_node = (struct vNode *)malloc(sizeof(struct vNode));
      new_u_node->prev = adjList[u].second;
      new_u_node->adj_v = y;
      new_u_node->edge = identified_edge_wy;
      new_u_node->special = true;
      new_u_node->segment = NULL;
      new_u_node->next = NULL;
      if (adjList[u].first == NULL)
        adjList[u].second = adjList[u].first = new_u_node;
      else
        adjList[u].second = adjList[u].second->next = new_u_node;
      if (identified_edge_wy->u == w)
      {
        identified_edge_wy->u = u;
        identified_edge_wy->Nu = new_u_node;
        identified_edge_wy->Nv->adj_v = u;
        if (dG[identified_edge_wy->v] <= 3)
          ddG[u]++;
      }
      else
      {
        identified_edge_wy->v = u;
        identified_edge_wy->Nv = new_u_node;
        identified_edge_wy->Nu->adj_v = u;
        if (dG[identified_edge_wy->u] <= 3)
          ddG[u]++;
      }
      remove_Pivot(w);
      if (ddG[u] > 0 && (dG[u] - ddG[u] <= 3) && VtoPivot[u] == NULL)
        VtoPivot[u] = add_Pivot(u);
      COLOR();
      // split_back
      if (identified_edge_wy->u == u)
      {
        identified_edge_wy->u = w;
        identified_edge_wy->Nu = identified_node_wy;
        identified_edge_wy->Nv->adj_v = w;
        if (dG[identified_edge_wy->v] <= 3)
          ddG[u]--;
      }
      else
      {
        identified_edge_wy->v = w;
        identified_edge_wy->Nv = identified_node_wy;
        identified_edge_wy->Nu->adj_v = w;
        if (dG[identified_edge_wy->u] <= 3)
          ddG[u]--;
      }
      adjList[w] = {identified_node_wy, identified_node_wy};
      dG[w] = 1;
      dG[u]--;
      if (adjList[u].first == new_u_node)
        adjList[u].first = new_u_node->next;
      if (adjList[u].second == new_u_node)
        adjList[u].second = new_u_node->prev;
      if (new_u_node->prev != NULL)
        new_u_node->prev->next = new_u_node->next;
      if (new_u_node->next != NULL)
        new_u_node->next->prev = new_u_node->prev;
      free(new_u_node);
      add_removed_Edge(edge_wx);
      add_removed_Edge(edge_vw);
      add_removed_Edge(edge_uv);
      // color wx
      if (Miss[x].first != NULL)
        coloring_with_Lemma5(edge_wx, Miss[x].first->color);
      else if (Once[x].first != NULL && Once[x].first->color != adjList[w].first->edge->color)
        coloring_with_Lemma5(edge_wx, Once[x].first->color);
      else if (Once[x].first != NULL && Once[x].first->next != NULL && Once[x].first->next->color != adjList[w].first->edge->color)
        coloring_with_Lemma5(edge_wx, Once[x].first->next->color);
      // color uv and vw
      if (Once[v].first != NULL && Once[v].first->next != NULL && Once[v].first->next->next == NULL)
      {
        int i = Once[v].first->color, j = Once[v].first->next->color, l = Once[v].first->next->next->color;
        struct vNode *adj_vu_i = Once[v].first->Nu;
        struct vNode *adj_vu_j = Once[v].first->next->Nu;
        struct vNode *adj_vu_l = Once[v].first->next->next->Nu;
        vector<int> L, sL;
        if (maximal_monochromatic_segment(v, adj_vu_i, u))
          L.push_back(i);
        else
          sL.push_back(i);
        if (maximal_monochromatic_segment(v, adj_vu_i, u))
          L.push_back(j);
        else
          sL.push_back(j);
        if (maximal_monochromatic_segment(v, adj_vu_i, u))
          L.push_back(l);
        else
          sL.push_back(l);
        if (L.size() == 2)
        {
          for (int p : L)
          {
            if (!check_Twice(w, p))
            {
              coloring_with_Lemma5(edge_vw, p);
              break;
            }
          }
          coloring_with_Lemma5(edge_uv, sL[0]);
        }
        else
        {
          int r = -1;
          if (!check_Colors(w, i))
            r = i;
          else if (!check_Colors(w, j))
            r = j;
          else if (!check_Colors(w, l))
            r = l;
          coloring_with_Lemma5(edge_vw, r);
          for (int q : sL)
          {
            if (!check_Twice(u, q) && q != r)
            {
              coloring_with_Lemma5(edge_uv, q);
              break;
            }
          }
        }
      }
      else if (Miss[v].first != NULL && Once[v].first != NULL)
      {
        int i = Miss[v].first->color, j = Once[v].first->color;
        struct vNode *adj_vu = Once[v].first->Nu;
        if (check_Twice(u, i))
        {
          coloring_with_Lemma5(edge_uv, j);
          coloring_with_Lemma5(edge_vw, i);
        }
        else if (check_Twice(w, i))
        {
          coloring_with_Lemma5(edge_vw, j);
          coloring_with_Lemma5(edge_uv, i);
        }
        else if (check_Twice(u, j) || check_Twice(w, j))
        {
          coloring_with_Lemma5(edge_uv, i);
          coloring_with_Lemma5(edge_vw, i);
        }
        else
        {
          //  if there is a maximal monochromatic segment having (v, ej ) as a terminal vertex-edge incidence and u as a terminal vertex
          if (maximal_monochromatic_segment(v, adj_vu, u))
          {
            coloring_with_Lemma5(edge_uv, i);
            coloring_with_Lemma5(edge_vw, j);
          }
          else
          {
            coloring_with_Lemma5(edge_uv, j);
            coloring_with_Lemma5(edge_vw, i);
          }
        }
      }
    }
    // cout<<"leaving 2nd condn\n";
  }
}

int main()
{
  /* Initialization | Inputs : an input graph G having maximum degree at most 2k − 1 */
  int M; // N = #vertices, M = #edges <= (3*N - 6)
  cout << "Input Format :\n First line of the input contains N and M where N = |V(G)| and M = |E(G)|\n Next M lines contains u and v where uv is an element of E(G)\n";
  cin >> N >> M;
  adj.resize(N);
  tN = N;
  int u, v;
  for (int i = 0; i < M; i++)
  {
    cin >> u >> v;
    u--;
    v--;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  Preprocessing(N);
  cout << "total colors : => K = " << K << "\n";
  // Show_Pivots();
  COLOR();
  cout << "\n---------------- End of Algorithm ----------------\n\n";
  Show_Edges();
  return 0;
}