#include <ECS.h>
#include <iostream>

#define STRINGIFY(x) (#x)

struct DataHolder {
    float x, y, z;
    std::string className = STRINGIFY(DataHolder);

    DataHolder() { 
        std::cout << "DataHolder()" << std::endl;
    }

    ~DataHolder() {
        std::cout << "~DataHolder()" << std::endl;
    }
};

void test() {
    auto dhx = ServiceLocator::get<DataHolder>();
    std::clog << "DHX: " << dhx->x << "\n";
}

int main() {
    
    auto entityManager = ServiceLocator::get<EntityManager>();
    auto entity1 = entityManager->New();
    auto entity2 = entityManager->New();

    entity1->AttachComponent<DataHolder>();
    entity2->AttachComponent<DataHolder>();

    auto c1 = entity1->GetComponent<DataHolder>();
    auto c2 = entity2->GetComponent<DataHolder>();

    c1->x = 123.456f;
    c2->x = 456.123f;


    std::clog << "c1->x = " << c1->x << "\n";
    std::clog << "c2->x = " << c2->x << "\n";

    {
        Component<DataHolder> c;
        auto dh1 = ServiceLocator::get<DataHolder>();
        auto dh2 = ServiceLocator::get<DataHolder>();

        std::clog << "Address REF DH1: " << &dh1 << "\n";
        std::clog << "Address REF DH2: " << &dh2 << "\n";

        dh1->x = 123;

        std::clog << "DH2: " << dh2->x << "\n";
    }

    auto dh = ServiceLocator::get<DataHolder>();

    dh->x = 123;
    test();
    dh->x += dh->x;
    test();
    dh->x += dh->x;
    test();

    {
        auto dh3 = ServiceLocator::get<DataHolder>();
        std::clog << "DH3: " << dh3->x << "\n";
    }

    return 0;
}