package com.catalin.lfpc_lab1;

import java.awt.*;
import javax.swing.*;
import java.awt.geom.*;
import java.awt.image.BufferedImage;
import java.util.Scanner;

import static java.lang.Character.*;

class G extends JPanel{
    int[] coordinates={100,20};
    int mar=50;
    protected void paintComponent(Graphics g){
        super.paintComponent(g);
        Graphics2D g1=(Graphics2D)g;
        g1.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
        int width=getWidth();
        int height=getHeight();
        g1.draw(new Line2D.Double(mar,mar,mar,height-mar));
        g1.draw(new Line2D.Double(mar,height-mar,width-mar,height-mar));
        double x=(double)(width-2*mar)/(coordinates.length-1);
        double scale=(double)(height-2*mar)/getMax();
        g1.setPaint(Color.BLUE);
        for(int i=0;i<coordinates.length;i++){
            double x1=mar+i*x;
            double y1=height-mar-scale*coordinates[i];
            g1.fill(new Ellipse2D.Double(x1-2,y1-2,4,4));
        }



    }
    private int getMax(){
        int max=-Integer.MAX_VALUE;
        for(int i=0;i<coordinates.length;i++){
            if(coordinates[i]>max)
                max=coordinates[i];

        }return max;
    }

    static char graphv[][] = new char[100][100];
    static char vertices[] = new char[100];
    static char termVar[] = new char[100];
    static String vs[] = new String[100];
    static String s1,s2,s3=" ";
    static int vertices_length = 0 , term_length = 0;

    static int find(char temp){
        if( isUpperCase(temp) ){
            for (int i=1;i<= vertices.length;i++) if (vertices[i] == temp) return i;}
        else{
            for (int i=1;i<= termVar.length;i++) if (termVar[i] == temp) return i;}
        return 0;
    }

    static void read(){
        System.out.print("Vn={ ");
        Scanner in = new Scanner(System.in);
        s1 = in.nextLine();
        System.out.println("}");
        System.out.print("Vt={ ");
        s2 = in.nextLine();
        System.out.println("}");
        System.out.print("P={ ");
        int n=1;
        s3 = in.nextLine();
        vs[1]=s3;
        while((s3.charAt(s3.length()-1) == ',' )||(s3.charAt(s3.length()-1) == ';')) {
            n++;
            s3 = in.nextLine();
            vs[n]=s3;
        }
        System.out.println("}");

        for(int i=0;i<s1.length();i++){
           if(isUpperCase(s1.charAt(i))){
               vertices_length++;
               vertices[vertices_length]=s1.charAt(i);

           }
        }
        for(int i=0;i<s2.length();i++){
            if(isLowerCase(s2.charAt(i))){
                term_length++;
                termVar[term_length]=s2.charAt(i);
            }
        }

        for(int i=1;i<=vertices_length;i++) {
            for (int j = 1; j <= vertices_length; j++) {
                graphv[i][j] = ' ';
            }
        }
        for(int i=1;i<=n;i++){
            boolean b=false;
            char firstVert = 'S';
            char firstChar = '*';
            for(int j=0;j<vs[i].length();j++){
                if (isLetter(vs[i].charAt(j)))
                    if ( b ) if ( isUpperCase ( vs[i].charAt(j))){
                        char c=vs[i].charAt(j);
                        graphv[find(firstVert)][find(c)]=firstChar ;
                             } else firstChar = vs[i].charAt(j);
                    else {firstVert = vs[i].charAt(j);b=true;}
            }
        }

    }

    static void draw(){
        System.out.print("  ");
        for(int i=1;i<=vertices_length;i++)System.out.print(vertices[i] + " ");
        System.out.println();
        for(int i=1;i<=vertices_length;i++){
            System.out.print(vertices[i] + " ");
            for(int j=1; j<=vertices_length; j++){
                System.out.print(graphv[i][j]+" ");
            }
            System.out.println(" ");
        }
    }

    public static void main(String args[]){
        read();
        draw();

        JFrame frame =new JFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(new G());
        frame.setSize(400,400);
        frame.setLocation(200,200);
        frame.setVisible(true);

    }
}
