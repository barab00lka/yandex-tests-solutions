#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NMAX 20

typedef struct {
    int tun;
    int treasure;
} island;

typedef struct {
    int rows;
    int columns;
    island **matrix;
} matrix_t;

// ---- Создание и удаление матрицы ----
int my_create_matrix(int rows, int columns, matrix_t *pM) {

    pM->columns = columns, pM->rows = rows;
	pM->matrix = (island **)calloc(rows * columns * sizeof(island) + rows, sizeof(island *));
    island *ptr = (island *)(pM->matrix + rows);
    for (int i = 0; i < rows; i++) pM->matrix[i] = ptr + columns * i;

    return 0;
}

void my_remove_matrix(matrix_t *A) {
    if (A != NULL && A->matrix != NULL) {
        // free(A->matrix[0]); // данные
        free(A->matrix);    // указатели
        A->matrix = NULL;
    }
}

// ---- Ввод туннелей ----
void get_paths(matrix_t *adj, int m, int *treasures) {
    for (int t = 0; t < m; ++t) {
        int a = 0, b = 0;
        scanf("%d %d", &a, &b);
        a--; b--; // 0-based
        adj->matrix[a][b].tun = 1;
        adj->matrix[b][a].tun = 1;
        adj->matrix[a][b].treasure = treasures[b];
        adj->matrix[b][a].treasure = treasures[a];
    }
}

// ---- DFS с DP по состояниям (маска + вершина) ----
void dfs(matrix_t *adj,
         int v,
         unsigned int mask,
         int cur_cost,
         int *best,
         int treasures[],
         int n,
         int **dp) {

    if (dp[mask][v] >= cur_cost) return; // уже был лучший путь сюда
    dp[mask][v] = cur_cost;

    if (cur_cost > *best) *best = cur_cost;

    for (int u = 0; u < n; u++) {
        if (adj->matrix[v][u].tun && !(mask & (1 << u))) {
            dfs(adj, u, mask | (1 << u), cur_cost + treasures[u], best, treasures, n, dp);
        }
    }
}

// ---- Главная программа ----
int main() {
    int n = 0, m = 0;
    scanf("%d %d", &n, &m);
    assert(n <= NMAX);

    int treasures[n];
    for (int i = 0; i < n; i++) scanf("%d", &treasures[i]);

    if (m == 0) {
        printf("%d\n", treasures[0]);
        return 0;
    }

    matrix_t adj;
    if (my_create_matrix(n, n, &adj) != 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    get_paths(&adj, m, treasures);

    // DP массив: выделяем в куче
    int **dp = malloc((1 << n) * sizeof(int*));
    for (int i = 0; i < (1 << n); i++) {
        dp[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) dp[i][j] = -1;
    }

    int best = treasures[0];
    dfs(&adj, 0, 1 << 0, treasures[0], &best, treasures, n, dp);

    printf("%d\n", best);

    // Освобождение памяти DP
    for (int i = 0; i < (1 << n); i++) free(dp[i]);
    free(dp);
    my_remove_matrix(&adj);

    return 0;
}
