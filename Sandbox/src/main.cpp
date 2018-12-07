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
        std::cout << "TestData(float,int)\n";
    }

    TestData() {
        x = mData2 = 1;
        std::cout << "TestData()\n";
    }

    ~TestData() {
        std::cout << "~TestData()\n";
    }

    void print() {
        printf("{\"data\": %f, \"data2\": %d}\n", x, mData2);
    }
};

struct TestD {
    float x;

    TestD() {
        printf("TestD\n");
    }
    
    ~TestD() {
        printf("~TestD\n");
    }
};

int main() {
    
    auto entityManager = new EntityManager();
    auto entity1 = entityManager->New();
    auto entity2 = entityManager->New();

    entity1->AttachComponent<TestData>(123.123, 123);
    entity2->AttachComponent<DataHolder>();

    entity1->AttachComponent<TestD>();
    entity2->AttachComponent<TestD>();
    std::clog << "test\n";

    entity1->destroy();

    return 0;
}