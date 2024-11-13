# ezvis (Acyclic Visitor in C++20)

<!--toc:start-->

- [ezvis (Acyclic Visitor in C++20)](#ezvis-acyclic-visitor-in-c20)
  - [0. Motivation](#0-motivation)
  - [1. How to use](#1-how-to-use)

<!--toc:end-->

## 0. Motivation

When one has an extensive class hierarchy it's convenient to be able to add operations for the objects of a hierarchy.
One classical approach to achieve this is the _Visitor_ pattern. For example, suppose you have a hierarchy of geometrical 2D shapes:

```cpp

class IVisitor;

class IShape {
  virtual void accept(IVisitor &) = 0;
  virtual ~IShape() {}
};

class Rectangle;
class Circle;

class IVisitor {
  virtual void visit(Rectangle &) = 0;
  virtual void visit(Circle &) = 0;
  virtual ~IVisitor() {}
};

struct Rectangle : public IShape {
  float m_width, m_height;
  
  void accept(IVisitor &visitor) override {
    visitor.visit(*this);
  }
};

struct Circle : public IShape {
  float m_radius;
  
  void accept(IVisitor &visitor) override {
    visitor.visit(*this);
  }
};

```

Classical Visitor pattern relies on the static resolution of correct visit overload. This means that all Derived classes should be known to the visitor.
One more inconvenience is the necessity to stick to a single return type of the `visit` function. This leads to hacky workarounds. As it turns out, it is possible
to make visitors very convenient to use.

Suppose you want to calculate the area of `IShape`.

```cpp

class AreaVisitor : public IVisitor {
  float area;

  void visit(Rectangle &ref) override { area = ref.m_width * ref.m_height; }
  void visit(Circle &ref) override { area = PI * ref.m_radius * ref.m_radius; }

  float area(IShape &ref) { visit(ref); return area; }
};

```

Using this library this code can be rewritten as:

```cpp

class IShape : ezvis::visitable_base<IShape> {
  EZVIS_VISITABLE();
  virtual ~IShape() {}
};

struct Rectangle : public IShape {
  EZVIS_VISITABLE();
  float m_width, m_height;
};

struct Circle : public IShape {
  EZVIS_VISITABLE();
  float m_radius;
};

template <class... Ts> struct Visitors : Ts... {
  using Ts::operator()...;
};

template <class... Ts> Visitors(Ts...) -> Visitors<Ts...>;

float area(IShape &ref) {
  return ezvis::visit<float, Rectangle, Circle>(
    Visitors{
      [](const Rectangle &ref) { return ref.m_width * ref.m_height; },
      [](const Circle &ref) { return PI * ref.m_radius * ref.m_radius; }
    },
    ref
  );
}

```

## 1. How to use

This library relies on [ctti](https://github.com/Manu343726/ctti). For example of how to use this library with CMake look at test/CMakeLists.txt.
