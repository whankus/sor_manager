#include "SorManager.h"
#include "Interface.h"

int main() {
    SorManager manager;
    Interface interfejs(manager);
    interfejs.uruchom();
    return 0;
}