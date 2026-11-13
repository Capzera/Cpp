abstract class Shape {
    abstract double Area();
    abstract double Perimeter();
}
class Rectangle extends Shape {
    double len;
    double wid;
    public Rectangle(double len, double wid) {
        this.len = len;
        this.wid = wid;
    }
    public double Area() {
        return len * wid;
    }
    public double Perimeter() {
        return (len + wid) * 2;
    }
    public void Print() {
        System.out.println("RecArea=" + Area());
        System.out.println("Perimeter=" + Perimeter());
    }
}
class Circle extends Shape {
    double r;
    final double PI = 3.14;

    public Circle(double r) {
        this.r = r;
    }
    public double Area() {
        return PI * r * r;
    }
    public double Perimeter() {
        return PI * r * 2;
    }
    public void Print() {
        System.out.println("Area=" + Area());
        System.out.println("Perimeter" + Perimeter());
    }
}
public class Test4 {
    public static void main(String[] args) {
        Rectangle r = new Rectangle(3.4, 5.6);
        Circle c = new Circle(5);
        r.Print();
        c.Print();
}