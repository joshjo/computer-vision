#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

   /* cout << _OPENMP << endl;
#pragma omp parallel num_threads(4)
  {
    int id = omp_get_thread_num();
    int data = id;
    int total = omp_get_num_threads();
    printf("Greetings from process %d out of %d with Data %d\n", id, total, data);
  }
  printf("parallel for ends.\n");
  return 0;*/
}
