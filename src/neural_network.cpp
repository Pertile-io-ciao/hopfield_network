/* #include "neural_network.hpp"
//#include "functions.hpp"

hebb::hebb(const std::vector<std::vector<int>>& v, std::string source) {
      // v è una matrice p x n
  int p = v.size();     // numero di pattern che voglio memorizzare
  int n = v[0].size();  // numero di neuroni
  weigh_matrix.resize(n, std::vector<float>(n, 0.0));  // inizializza la matrice n x n, tutta 0
  for (int i = 0; i < n; ++i) {    // due cicli per considerare ogni neurone
    for (int j = 0; j < n; ++j) {
      int sum = 0;
      for (int k = 0; k < p; ++k) {
        sum += v[k][i] * v[k][j];
      }  // la sommatoria della regola
      if (i != j) {
        weigh_matrix[i][j] = sum / n;  // regola completa
      } else {
        weigh_matrix[i][j] = 0;  // niente auto-connessioni
      }
    }
  }

  this->sourceFolder = source;
}
  */