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

struct TestData {

private:
    int mData2;

public:
    float x;
    TestData(float asd, int qwe) {
        x = asd;
        mData2 = qwe;
    }

    TestData() {
        x = mData2 = 1;
    }

    void print() {
        printf("{\"data\": %f, \"data2\": %d}\n", x, mData2);
    }
};

struct TestD {
    float x;
};

int main() {
    
    auto entityManager = ServiceLocator::get<EntityManager>();
    auto entity1 = static_cast<Entity*>(entityManager->New());
    auto entity2 = static_cast<Entity*>(entityManager->New());

    entity1->AttachComponent<TestData>(123.123, 123);
    entity2->AttachComponent<DataHolder>();

    auto c1 = entity1->GetComponent<TestData>();
    auto c2 = entity2->GetComponent<DataHolder>();

    c1->x = 123.456f;
    c2->x = 456.123f;


    std::clog << "c1->x = " << c1->x << "\n";
    std::clog << "c2->x = " << c2->x << "\n";

    entity1->AttachComponent<TestD>();
    entity2->AttachComponent<TestD>();

    


    // {
    //     Component<DataHolder> c;
    //     auto dh1 = ServiceLocator::get<DataHolder>();
    //     auto dh2 = ServiceLocator::get<DataHolder>();

    //     std::clog << "Address REF DH1: " << &dh1 << "\n";
    //     std::clog << "Address REF DH2: " << &dh2 << "\n";

    //     dh1->x = 123;

    //     std::clog << "DH2: " << dh2->x << "\n";
    // }

    // auto dh = ServiceLocator::get<DataHolder>();

    // dh->x = 123;
    // test();
    // dh->x += dh->x;
    // test();
    // dh->x += dh->x;
    // test();

    // {
    //     auto dh3 = ServiceLocator::get<DataHolder>();
    //     std::clog << "DH3: " << dh3->x << "\n";

    //     auto ci1 = entity1->GetComponent<DataHolder>();
    //     auto ci2 = entity2->GetComponent<DataHolder>();

    //     ci1->x = 9798127938;
    //     ci2->x = 1235675;
    // }

    std::clog << "c1->x = " << c1->x << "\n";
    std::clog << "c2->x = " << c2->x << "\n";

    return 0;
}