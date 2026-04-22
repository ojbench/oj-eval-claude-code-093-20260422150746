#include <iostream>
#include <vector>
#include <set>
#include <tuple>

using namespace std;

// Directions: 0: Up, 1: Right, 2: Down, 3: Left
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

struct State {
    int x, y, dir;
    vector<bool> beans; // State of beans (eaten or not)

    bool operator<(const State& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        if (dir != other.dir) return dir < other.dir;
        return beans < other.beans;
    }
};

int main() {
    vector<vector<int>> grid(10, vector<int>(10));
    int startX, startY;
    vector<pair<int, int>> beanPositions;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            cin >> grid[i][j];
            if (grid[i][j] == 4) {
                startX = i;
                startY = j;
            } else if (grid[i][j] == 2) {
                beanPositions.push_back({i, j});
            }
        }
    }

    int currentX = startX;
    int currentY = startY;
    int currentDir = 0; // Starts moving UP
    int score = 0;
    vector<bool> currentBeans(beanPositions.size(), false);

    // Initial check for bean at start (though problem says 4 is start and unique)
    // Actually, check if current position has a bean.
    for (int b = 0; b < beanPositions.size(); ++b) {
        if (beanPositions[b].first == currentX && beanPositions[b].second == currentY) {
            if (!currentBeans[b]) {
                score += 2;
                currentBeans[b] = true;
            }
        }
    }

    set<State> visited;

    while (true) {
        State currentState = {currentX, currentY, currentDir, currentBeans};
        if (visited.count(currentState)) {
            cout << "Silly Pacman" << endl;
            return 0;
        }
        visited.insert(currentState);

        // Check for ghosts
        if (grid[currentX][currentY] == 3) {
            score -= 500;
            cout << score << endl;
            return 0;
        }

        // Determine next direction based on Right-Hand Rule
        // Right > Straight > Left > Back
        int nextDir = -1;
        int priorities[] = {1, 0, 3, 2}; // Relative to currentDir: Right (+1), Straight (+0), Left (+3/ -1), Back (+2)

        for (int p : priorities) {
            int d = (currentDir + p) % 4;
            int nx = currentX + dx[d];
            int ny = currentY + dy[d];

            if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && grid[nx][ny] != 0) {
                nextDir = d;
                break;
            }
        }

        if (nextDir == -1) {
            // No available moves (shouldn't happen with walls at border and Back option, but safety first)
            cout << score << endl;
            return 0;
        }

        // Move to next position
        currentX += dx[nextDir];
        currentY += dy[nextDir];
        currentDir = nextDir;

        // Check for beans
        if (grid[currentX][currentY] == 2) {
            for (int b = 0; b < beanPositions.size(); ++b) {
                if (beanPositions[b].first == currentX && beanPositions[b].second == currentY) {
                    if (!currentBeans[b]) {
                        score += 2;
                        currentBeans[b] = true;
                    }
                    break;
                }
            }
        }

        // Ghost check at new position
        if (grid[currentX][currentY] == 3) {
            score -= 500;
            cout << score << endl;
            return 0;
        }
    }

    return 0;
}
