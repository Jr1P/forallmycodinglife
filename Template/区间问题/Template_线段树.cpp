#include <iostream>
#include <cstdio>
using namespace std;
const int MAXN = 1e6+10;

typedef long long ll;

int p, x, y;
ll ans = 0, add;
//p为待查询或修改点,[x,y]为待查询或修改区间,ans区间查询的答案, 使用前初始化为0 add为增值
struct node
{
	int l, r, lazy; // [l, r], w-weight为权重, lazy为懒标记
	ll w;
}tree[MAXN<<2];

void build(int l, int r, int k);
	//建树, k为节点序号 初始状态 (1, n, 1)
inline void down(int k);
	//懒标记下传
ll nodeInquiry(int k); 
	//单点查询, p为待查询点 
void intervalInquiry(int k); 
	//区间查询, 查询区间为 [x, y], k为当前节点序号 初始 (1); 可以将查询区间设为全局变量以减少函数的参数量
void nodeModify(int k);
	//单点修改, 在x节点上加add, k为当前节点 初始 (1, add, x)
void intervalModify(int k);
	//区间修改,[l, r]为待修改区间,k为当前节点,初始 (l, r, 1, add)
int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	build(1, n, 1);
	int q;
	while (m--) {
		scanf("%d%d%d", &q, &x, &y);
		if (q == 1) {
			scanf("%lld", &add);
			if (x == y)
				nodeModify(1);
			else
				intervalModify(1);
		}
		else {
			ans = 0;
			if (x == y)
				ans = nodeInquiry(1);
			else
				intervalInquiry(1);
			printf("%lld\n", ans);
		}
	}
	return 0;
}

void build(int l, int r, int k) {
	tree[k].l = l; tree[k].r = r;
	if (l == r) {
		scanf("%lld", &tree[k].w);
		return ;
	}
	int mid = (l + r) >> 1;
	int ls = k << 1, rs = k << 1 | 1;
	build(l, mid, ls);
	build(mid+1, r, rs);
	tree[k].w = tree[ls].w + tree[rs].w;
}

inline void down(int k)
{
	int ls = k<<1, rs = k << 1|1;
	tree[ls].lazy += tree[k].lazy;
	tree[rs].lazy += tree[k].lazy;
	tree[ls].w += tree[k].lazy*(tree[ls].r-tree[ls].l+1);
	tree[rs].w += tree[k].lazy*(tree[rs].r-tree[rs].l+1);
	tree[k].lazy = 0;
}

ll nodeInquiry(int k)
{
	if (tree[k].l == tree[k].r)
		return tree[k].w;
	if (tree[k].lazy)
		down(k);
	int mid = (tree[k].l + tree[k].r) >> 1;
	if (p <= mid) 
		return nodeInquiry(k<<1);
	else 
		return nodeInquiry(k<<1|1);
}

void nodeModify(int k) {
	if (tree[k].l == tree[k].r) {
		tree[k].w += add;
		return ;
	}
	if (tree[k].lazy)
    	down(k);
	int mid = (tree[k].l + tree[k].r) >> 1;
	if (p <= mid) 
		nodeModify(k<<1);
	else 
		nodeModify(k<<1|1);
	tree[k].w = tree[k<<1].w + tree[k<<1|1].w;
}

void intervalInquiry(int k) {
	if ((tree[k].l >= x) && (tree[k].r <= y)) {
		ans += tree[k].w;
		return ;
	}
	if (tree[k].lazy)
		down(k);
	int mid = (tree[k].l + tree[k].r) >> 1;
	if (x <= mid) 
		intervalInquiry(k<<1);
	if (y > mid)
		intervalInquiry(k<<1|1);
}

void intervalModify(int k)
{
	if (tree[k].l >= x && tree[k].r <= y)
	{
		tree[k].w += (tree[k].r - tree[k].l + 1) * add;
		tree[k].lazy += add;
		return ;
	}
	if (tree[k].lazy)
		down(k);
	int mid = (tree[k].l + tree[k].r) >> 1;
	if (x <= mid)
		intervalModify(k<<1);
	if (y > mid)
		intervalModify(k<<1|1);
	tree[k].w = tree[k<<1].w + tree[k<<1|1].w;
}