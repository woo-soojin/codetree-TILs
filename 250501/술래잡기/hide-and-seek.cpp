#include <iostream>
#include <vector>
using namespace std;

int grid[100][100];

struct Runner {
	int x;
	int y;
	int dir; // 1, 2
	bool isDisappear = false;
};

// 상 우 하 좌
int dx[4] = { -1,0,1,0 };
int dy[4] = { 0,1,0,-1 };

// 하 우 상 좌
int rx[4] = { 1,0,-1,0 };
int ry[4] = { 0,1,0,-1 };

int N, M, H, K;
int sx, sy; // TODO
int cDir = 0;
int ans = 0;

vector<Runner> runners;
vector<pair<int, int>> trees;

void moveRunner() {
	for (Runner& runner : runners) {
		if (runner.isDisappear)
			continue;

		int cx = runner.x;
		int cy = runner.y;
		int cDir = runner.dir;

		int dist = abs(sx - cx) + abs(sy - cy);

		if (dist > 3)
			continue;

		int nx = cx + dx[cDir];
		int ny = cy + dy[cDir];

		if (nx < 0 || ny < 0 || nx >= N || ny >= N) {
			cDir = (cDir + 2) % 4;
			runner.dir = cDir;

			nx = cx + dx[cDir];
			ny = cy + dy[cDir];

			if (nx == sx && ny == sy)
				continue;
			else {
				runner.x = nx;
				runner.y = ny;
			}
		}
		else {
			if (nx == sx && ny == sy)
				continue;
			else {
				runner.x = nx;
				runner.y = ny;
			}
		}
	}
}

int catchRunner(bool isClockWise, int dir) {
	int catched = 0;
	int cx = sx;
	int cy = sy;

	if (isClockWise) {
		for (int d = 1; d <= 3; d++) {
			int nx = cx + dx[dir] * d;
			int ny = cy + dy[dir] * d;

			if (nx < 0 || ny < 0 || nx >= N || ny >= N)
				continue;

			for (pair<int, int> tree : trees) {
				int tx = tree.first;
				int ty = tree.second;

				if (nx == tx && ny == ty)
					continue;

				for (Runner& runner : runners) {
					if (runner.isDisappear)
						continue;

					if (nx == runner.x && ny == runner.y) {
						catched++;
						runner.isDisappear = true;
					}
				}
			}
		}
	}
	else {
		for (int d = 1; d <= 3; d++) {
			int nx = cx + rx[dir] * d;
			int ny = cy + ry[dir] * d;

			if (nx < 0 || ny < 0 || nx >= N || ny >= N)
				continue;

			for (pair<int, int> tree : trees) {
				int tx = tree.first;
				int ty = tree.second;

				if (nx == tx && ny == ty)
					continue;

				for (Runner& runner : runners) {
					if (runner.isDisappear)
						continue;

					if (nx == runner.x && ny == runner.y) {
						catched++;
						runner.isDisappear = true;
					}
				}
			}
		}
	}

	return catched;
}

void moveSeeker(bool& isClockWise, int& dist, int& dir, bool& flag) {
	int nx, ny;
	if (sx == N / 2 && sy == N / 2) {
		isClockWise = true;
		dir = 0;
		dist = 1;
	}

	if (sx == 0 && sy == 0) {
		isClockWise = false;
		dir = 1;
		dist = 4;
		sx = N - 1;
		sy = 0;
		return;
	}

	if (dist == N) {
		sx = 0;
		sy = 0;
		dir = 2; // TODO
		return;
	}

	// 두번반복
	if (isClockWise) {
		nx = sx + dx[dir] * dist;
		ny = sy + dy[dir] * dist;
	}
	else {
		nx = sx + rx[dir] * dist;
		ny = sy + ry[dir] * dist;
	}

	sx = nx;
	sy = ny;

	dir = (dir + 1) % 4;

	if (flag == false)
		flag = true;
	else {
		if (isClockWise)
			dist++;
		else
			dist--;
		flag = false;
	}
}

void run() {
	//int nx, ny;
	int dist = 1;
	int dir = 0;
	bool isClockWise = true;
	bool flag = false;

	for (int k = 1; k <= K; k++) {
		moveRunner();

		moveSeeker(isClockWise, dist, dir, flag);

		int catched = catchRunner(isClockWise, dir);
		ans += catched * k;
	}
}

int main() {
	cin >> N >> M >> H >> K;

	// init
	sx = N / 2;
	sy = N / 2;
	grid[sx][sy] = 1;

	Runner runner;
	for (int m = 1; m <= M; m++) {
		cin >> runner.x >> runner.y >> runner.dir;
		runner.x = runner.x - 1;
		runner.y = runner.y - 1;

		runners.push_back(runner);
	}

	int tx, ty;
	for (int h = 1; h <= H; h++) {
		cin >> tx >> ty;
		tx = tx - 1;
		ty = ty - 1;

		trees.push_back({ tx,ty });
	}

	run();
	cout << ans << endl;

	return 0;
}
