#include<bits/stdc++.h>


#define pii pair<int,int>
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define eb emplace_back
#define pf push_front
#define pb2 pop_back
#define pf2 pop_front
#define line printf("\n")
#define pq priority_queue
#define rep(k,i,j) for(int k = (int)i;k<(int)j;k++)
#define repd(k,i,j) for(int k = (int)i;k>=(int)j;k--)
#define ll long long
#define ALL(a) a.begin(),a.end()
#define vi vector<int>

using namespace std;

double EPS = 1e-9;
int INF = 1e9+7;;
long long INFLL = 1e17;
double pi = acos(-1);
int dirx[8] = {-1,0,0,1,-1,-1,1,1};
int diry[8] = {0,1,-1,0,-1,1,-1,1};

clock_t first_attempt = clock();
inline void cek_time(){
	clock_t cur = clock()- first_attempt;
	cerr<<"TIME : "<<(double) cur/CLOCKS_PER_SEC<<endl;
}
inline void OPEN (string s) {
	freopen ((s + ".in").c_str (), "r", stdin);
	freopen ((s + ".out").c_str (), "w", stdout);
}

#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
typedef tree<pii, null_type, less<pii>, rb_tree_tag, tree_order_statistics_node_update> bbst;

//end of template

const int maxn = 5e5+5;
int n;
int val[maxn];

int par[maxn][20],hi[maxn];
int in[maxn],rev[maxn],hit;
int head[maxn],sz[maxn];

vi adj[maxn];
void dfs_lca(int a,int p){
	par[a][0] = p;
	hi[a] = hi[p] + 1;
	
	sz[a] = 1;
	for(auto b : adj[a]) if(b!=p){
		dfs_lca(b,a);
		sz[a] += sz[b];
	}
}

int up(int a,int pts){
	repd(k,19,0)if((1<<k)<=pts){
		pts -= (1<<k);
		a = par[a][k];
	}
	return a;
}

int lca(int a,int b){
	if(hi[a]<hi[b])swap(a,b);
	int dis = abs(hi[a] - hi[b]);
	rep(k,0,20)if(dis&(1<<k))a = par[a][k];
	if(a==b)return a;
	repd(k,19,0)if(par[a][k]!=par[b][k]){
		a = par[a][k];
		b = par[b][k];
	}
	return par[a][0];
}

void setup_lca(){
	dfs_lca(1,0);
	rep(k,1,20)rep(i,1,n+1){
		int j = par[i][k-1];
		par[i][k] = par[j][k-1];
	}
}

void dfs_hld(int a,int p,int _head){
	in[a] = ++hit;
	rev[hit] = a;
	head[a] = _head;
	rep(k,0,adj[a].size())if(sz[adj[a][k]] > sz[adj[a][0]])swap(adj[a][k],adj[a][0]);
	
	int ft = 1;
	for(auto b : adj[a])if(b!=p){
		if(ft)dfs_hld(b,a,_head), ft = 0;
		else dfs_hld(b,a,b);
	}
}

struct node{//change here
	int l,r,ans;
	
	node(){l = r = ans  = 0;}
	node(int l,int r,int ans):l(l),r(r),ans(ans){}
};

node merge(node a,node b){//change here
	node ret;
    if(a.l==0)return b;
    if(b.l==0)return a;
	ret.ans = a.ans + b.ans + (a.r==b.l?-1:0);
	ret.l = a.l;
	ret.r = b.r;
	return ret;
}

struct segtree{
	node arr[4*maxn];
	int lazy[4*maxn];
    
    void propagate(int a){
        if(!lazy[a])return;
        lazy[2*a] = lazy[2*a+1] = lazy[a];
        arr[a] = node(lazy[a],lazy[a],1);
        lazy[a] = 0;
    }
	void build(int a,int ki,int ka){//change here
		if(ki==ka){
			int nd = rev[ki];
			arr[a] = node(val[nd],val[nd],1);
		}
		else{
			int mid = (ki+ka)>>1;
			build(2*a,ki,mid);
			build(2*a+1,mid+1,ka);
			arr[a] = merge(arr[2*a],arr[2*a+1]);
		}
	}
	
	void build(){
		build(1,1,n);
	}
	
	int l,r,v;
	node query(int a,int ki,int ka){
        propagate(a);
		if(ka<l || ki>r)return node();
		if(l<=ki && ka<=r)return arr[a];
		int mid = (ki+ka)>>1;
		return merge(query(2*a,ki,mid),query(2*a+1,mid+1,ka));
	}
	node query(int ki,int ka){
		l = ki,r = ka;
		if(l>r)swap(l,r);
		return query(1,1,n);
	}
    
    void upd(int a,int ki,int ka){
        propagate(a);
        if(ka<l || ki>r)return;
        if(l<=ki && ka<=r){
            lazy[a] = v;
            propagate(a);
            return;
        }
        int mid = (ki+ka)>>1;
        upd(2*a,ki,mid);
        upd(2*a+1,mid+1,ka);
        arr[a] = merge(arr[2*a],arr[2*a+1]);
    }
    
    void update(int ki,int ka,int vv){
        l = ki, r = ka, v = vv;
        if(l>r)swap(l,r);
        //puts("haha");
        upd(1,1,n);
    }
}seg;

node query(int a,int b){
	int ab = lca(a,b);
	if(ab!=a && ab!=b){
		int diff = abs(hi[b] - hi[ab]);
		node l = query(a,ab),r = query(b,up(b,diff - 1));
        //printf("dbg %d %d :%d - %d %d : %d\n", l.l, l.r, l.ans, r.l, r.r, r.ans);
        swap(l.l,l.r);
		return merge(l,r);
	}
	else{
		if(ab==a)swap(a,b);
		node ret = node();
		while(hi[a]>=hi[ab]){
			int pa = head[a];
			if(hi[pa]<hi[b])pa = b;
			
			node cur = seg.query(in[a],in[pa]);
			ret = merge(cur,ret);
			
			a = par[pa][0];
		}
		return ret;
	}
}

void update(int a,int b,int v){
    
	int ab = lca(a,b);
	if(ab!=a && ab!=b){
		int diff = abs(hi[b] - hi[ab]);
		update(a,ab,v),update(b,up(b,diff - 1),v);
	}
	else{
		if(ab==a)swap(a,b);
		node ret;
		while(hi[a]>=hi[ab]){
            
			int pa = head[a];
			if(hi[pa]<hi[b])pa = b;
			seg.update(in[a],in[pa],v);
			a = par[pa][0];
		}
	}
}

void initHLD(){
    hit = 0;
    setup_lca();
	dfs_hld(1,0,1);
	seg.build();
}

int main(){
    int q;
    while(scanf("%d%d",&n,&q)!=EOF){
        rep(k,0,n+1)adj[k].clear();
        rep(k,1,n+1)scanf("%d",&val[k]);
        
        rep(k,1,n){
            int a,b;
            scanf("%d %d",&a,&b);
            adj[a].pb(b);
            adj[b].pb(a);
        }
        
        initHLD();
        
        rep(qq,0,q){
            char tp[3];
            int a,b,v;
            scanf("%s %d %d",tp,&a,&b);
            if(tp[0]=='C'){
                scanf("%d",&v);
                update(a,b,v);
            }
            else printf("%d\n",query(a,b).ans);
            
        }
    }
}