/* ASA projeto 1 2023/2024
 * Grupo: al039
 * Alunos: Sofia Lopes (106198), Raquel Rodrigues (106322)
 * 
 * Programa que calcula o valor máximo que pode ser obtido a patir de uma chapa 
 * de mármore cortando-a em peças com dimensões e preços fornecidos.
 */

#include <iostream>
#include <vector>
#include <algorithm>

int maxRectangleValue(int width, int height, std::vector<std::vector<int>>& table) {

    for (int i = 1 ; i <= width ; i++) {
        for (int j = 1 ; j <= height ; j++) {
            int maxHorizontalCuts = 0;
            int maxVerticalCuts = 0;
            int maxValue = 0;

            // Vertical cuts
            for (int p = 1 ; p <= i/2 ; p++) {
                maxVerticalCuts = std::max(maxVerticalCuts, table[p][j] + table[i-p][j]);
            }
            // Horizontal cuts
            for (int k = 1 ; k <= j/2 ; k++) {
                maxHorizontalCuts = std::max(maxHorizontalCuts, table[i][k] + table[i][j-k]);
            }

            maxValue = std::max(maxHorizontalCuts,maxVerticalCuts);

            table[i][j] = std::max(table[i][j], maxValue);
        }
    }
    return table[width][height];
}

int main(){
    int numPieces;  // Number of available marble pieces
    int maxWidth, maxHeight; // Width and height of the original marble slab
    int x, y; // Dimensions of one marble rectangle
    int price;

    if (scanf("%d %d", &maxWidth, &maxHeight) != 2) return 1;
    if (scanf("%d", &numPieces) != 1 ) return 1;

    // Create a table and initialize it with 0
    std::vector<std::vector<int>> table(maxWidth + 1 , std::vector<int>(maxHeight + 1, 0));

    // Obtain the available rectangles
    for (int i = 0; i < numPieces; i++) {
        if (scanf("%d %d %d", &x, &y, &price) != 3 ) return 1;

        if (price > 0 && x > 0 && y > 0 && ((x <= maxWidth && y <= maxHeight) || (y <= maxWidth && x <= maxHeight))) {
            // check if the rectangle is bigger than the marble slab

            if (x > maxWidth || y > maxHeight) { 
                // rotate the rectangle if it does not fit this way
                int aux = x; 
                x = y;
                y = aux;
            }
            if (table[x][y] == 0){ // empty table position 
                if (x <= maxWidth && y <= maxHeight) table[x][y] = price;
                if (x <= maxHeight && y <= maxWidth)  table[y][x] = price;
            }
            else { // there is already a price associated with these dimensions 
                if (x <= maxWidth && y <= maxHeight && table[x][y] < price) table[x][y] = price;
                if (x <= maxHeight && y <= maxWidth && table[y][x] < price)  table[y][x] = price;
            }
        }
    }

    int result = maxRectangleValue(maxWidth, maxHeight, table);

    std::cout << result << std::endl;

    return 0;
}