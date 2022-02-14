#include <iostream>
#include <vector>
#include <map>
#include "Singleton.hpp"
using namespace std;

// 1. 제품 생산하는 tatic 함수.
// 2. 제품 등록 객체(제품의 코드와, 제품을 생산하는 static 함수 포인터를 생성자 인자로 받는다.)


class Shape
{
public:
    virtual void Draw() { cout << "Draw Shape" << endl;}
};

class ShapeFactory  // 공장
{
    MAKE_SINGLETON(ShapeFactory)
    typedef Shape* (*CREATOR)();
    map<int, CREATOR> create_map; // 제품 목록 (코드와 해당 제품 종류 생산하는 함수포인터)
public:
    void Register( int type, CREATOR f )
    {
        create_map[type] = f;
    }

    Shape* CreateShape(int type ) // 제품 생산
    {
        Shape* p = 0;
        auto ret = create_map.find( type );
        if ( ret == create_map.end() )
            return 0;
        p = create_map[type](); // 제품 생산하는 함수 호출

        return p;
    }
};

struct RegisterShape // 제품 등록을 위한 객체.
{
    RegisterShape( int type, Shape*(*f)() )
    {
        ShapeFactory& factory = ShapeFactory::getInstance();

        factory.Register(type, f);
    }
};
//RegisterShape rs(1, &Rect::Create);



class Rect : public Shape
{
public:
    virtual void Draw() { cout << "Draw Rect" << endl;}

    static Shape* Create() { return new Rect;}  // 제품 생산하는 static 함수.
    static RegisterShape rs;  // 제품 등록 위한 객체.
};
RegisterShape Rect::rs( 1, &Rect::Create); // 제품 등록 위한 객체가 static 이므로 밖에서 한번 선언한다. 이 때 제품 코드와 생산 함수 포인터를 전달한다.


// 모든 도형이 지켜야 하는 규칙을 매크로로 제공
#define DECLARE_SHAPE( classname )                  \
    static Shape* Create() { return new classname;}      \
    static RegisterShape rs;

#define IMPLEMENT_SHAPE( type, classname )                \
    RegisterShape classname::rs(type, &classname::Create);


class Circle : public Shape
{
public:
    virtual void Draw() { cout << "Circle Rect" << endl;}

    DECLARE_SHAPE( Circle )
};
IMPLEMENT_SHAPE( 2, Circle )


class Triangle : public Shape
{
public:
    virtual void Draw() { cout << "Triangle Rect" << endl;}

    DECLARE_SHAPE( Triangle )
};
IMPLEMENT_SHAPE( 3, Triangle )




int main()
{
    ShapeFactory& factory = ShapeFactory::getInstance();


    vector<Shape*> v;

    while( 1 )
    {
        int cmd;
        cin >> cmd;

        //
        if ( cmd >=1 && cmd <= 5 )
        {
            Shape* p = factory.CreateShape(cmd);

            if ( p != 0 )
                v.push_back( p );
        }
        else if ( cmd == 9 )
        {
            for ( auto p : v )
                p->Draw(); // 다형성
        }
    }
}
































//
