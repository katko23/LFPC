package com.catalin.lfpc_lab1;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ContentDisplay;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.CubicCurve;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.Scanner;
import java.util.concurrent.atomic.AtomicReference;

import static java.lang.Character.*;

class LFPC {
    static String graphv[][] = new String[100][100];
    static char vertices[] = new char[100];
    static char termVar[] = new char[100];
    static String vs[] = new String[100];
    static String s1, s2, s3 = " ";
    static int vertices_length = 0, term_length = 0,n=1 , nOfEndPoints = 0 , nOfPoints = 0 , max=1;

    static int find(char temp) {
        if (isUpperCase(temp)) {
            for (int i = 1; i <= vertices.length; i++) if (vertices[i] == temp) return i;
        } else {
            for (int i = 1; i <= termVar.length; i++) if (termVar[i] == temp) return i;
        }
        return 0;
    }

    static void read() {
        System.out.print("Vn={ ");
        Scanner in = new Scanner(System.in);
        s1 = in.nextLine();
        System.out.println("}");
        System.out.print("Vt={ ");
        s2 = in.nextLine();
        System.out.println("}");
        System.out.print("P={ ");
        s3 = in.nextLine();
        vs[1] = s3;
        while ((s3.charAt(s3.length() - 1) == ',') || (s3.charAt(s3.length() - 1) == ';')) {
            n++;
            s3 = in.nextLine();
            vs[n] = s3;
        }
        System.out.println("}");

        for (int i = 0; i < s1.length(); i++) {
            if (isUpperCase(s1.charAt(i))) {
                vertices_length++;
                vertices[vertices_length] = s1.charAt(i);

            }
        }
        for (int i = 0; i < s2.length(); i++) {
            if (isLowerCase(s2.charAt(i))) {
                term_length++;
                termVar[term_length] = s2.charAt(i);
            }
        }

        for (int i = 1; i <= vertices_length; i++) {
            for (int j = 1; j <= vertices_length; j++) {
                graphv[i][j] = " ";
            }
        }
        for (int i = 1; i <= n; i++) {
            boolean b = false;
            char firstVert = 'S';
            char firstChar = '*';
            for (int j = 0; j < vs[i].length(); j++) {
                if (isLetter(vs[i].charAt(j)))
                    if (b) if (isUpperCase(vs[i].charAt(j))) {
                        char c = vs[i].charAt(j);
                        if(graphv[find(firstVert)][find(c)] == " ")
                        graphv[find(firstVert)][find(c)] =""+firstChar;
                        else {
                            graphv[find(firstVert)][find(c)] = graphv[find(firstVert)][find(c)] + "," +firstChar;
                            max++;
                        }
                    } else firstChar = vs[i].charAt(j);
                    else {
                        firstVert = vs[i].charAt(j);
                        b = true;
                    }
            }
        }

    }

    static void draw() {
        System.out.print("  ");
        String spatii="";
        for(int i=1;i<=2*max;i++)spatii=spatii+" ";
        for (int i = 1; i <= vertices_length; i++) System.out.print(vertices[i] + spatii);
        System.out.println();
        for (int i = 1; i <= vertices_length; i++) {
            System.out.print(vertices[i] + " | ");
            for (int j = 1; j <= vertices_length; j++) {
                System.out.print(graphv[i][j] + " | ");
                nOfPoints++;
            }
            System.out.println(" ");
        }
        System.out.println(" Start point is S");
        System.out.print(" End points is ");
        for(int i = 1;i<=n;i++) {
            if (isLowerCase(LFPC.vs[i].charAt(LFPC.vs[i].length() - 2))) {
                System.out.print(vs[i].charAt(0) + " , ");
                nOfEndPoints++;
            }
            if (isLowerCase(LFPC.vs[i].charAt(LFPC.vs[i].length() - 1))) {
                System.out.println(vs[i].charAt(0) + " .");
                nOfEndPoints++;
            }
        }
    }
}

public class HelloApplication extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(HelloApplication.class.getResource("hello-view.fxml"));
        try {

            Button[] roundButton = new Button[LFPC.vertices_length];
            TextField vert1Txt = new TextField("");
            vert1Txt.setPrefSize(800,10);


            for(int i=0;i<LFPC.vertices_length;i++){
                String tempS = ""+LFPC.vertices[i+1];
                roundButton[i] = new Button(tempS);
                Image img = new Image("D:/Work/UTM/Intelij/LFPC_lab1/src/main/java/com/catalin/lfpc_lab1/Resources/cerc.png");
                ImageView view = new ImageView(img);
                view.setFitHeight(30);
                view.setPreserveRatio(true);
                Image imgf = new Image("D:/Work/UTM/Intelij/LFPC_lab1/src/main/java/com/catalin/lfpc_lab1/Resources/end.png");
                ImageView viewf = new ImageView(imgf);
                viewf.setFitHeight(30);
                viewf.setPreserveRatio(true);
                Image imgs = new Image("D:/Work/UTM/Intelij/LFPC_lab1/src/main/java/com/catalin/lfpc_lab1/Resources/start.png");
                ImageView views = new ImageView(imgs);
                views.setFitHeight(30);
                views.setPreserveRatio(true);
                Image imgse = new Image("D:/Work/UTM/Intelij/LFPC_lab1/src/main/java/com/catalin/lfpc_lab1/Resources/st_end.png");
                ImageView viewse = new ImageView(imgse);
                viewse.setFitHeight(30);
                viewse.setPreserveRatio(true);

            roundButton[i].setStyle(
                    "-fx-background-radius: 5em; " +
                            "-fx-min-width: 30px; " +
                            "-fx-min-height: 30px; " +
                            "-fx-max-width: 30px; " +
                            "-fx-max-height: 30px;"
            );

            roundButton[i].setLayoutX(100+i*100);
            roundButton[i].setLayoutY(100+i*100);
            roundButton[i].setGraphic(view);
            roundButton[i].setContentDisplay(ContentDisplay.CENTER);
            roundButton[i].setText(tempS);
                for(int o=1;o<=LFPC.n;o++) {
                    if (LFPC.vs[o].charAt(0) == LFPC.vertices[i + 1] && isLowerCase(LFPC.vs[o].charAt(LFPC.vs[o].length()-2))){
                        roundButton[i].setGraphic(viewf);
                        roundButton[i].setContentDisplay(ContentDisplay.CENTER);}
                    if (LFPC.vs[o].charAt(0) == LFPC.vertices[i + 1] && isLowerCase(LFPC.vs[o].charAt(LFPC.vs[o].length()-1))){
                        roundButton[i].setGraphic(viewf);
                        roundButton[i].setContentDisplay(ContentDisplay.CENTER);}
                    if (LFPC.vs[o].charAt(0) == LFPC.vertices[i + 1] && LFPC.vs[o].charAt(0) == 'S' ){
                        if (LFPC.vs[o].charAt(0) == LFPC.vertices[i + 1] && isLowerCase(LFPC.vs[o].charAt(LFPC.vs[o].length()-2))){
                            roundButton[i].setGraphic(viewse);
                            roundButton[i].setContentDisplay(ContentDisplay.CENTER);
                        } else{
                        roundButton[i].setGraphic(views);
                        roundButton[i].setContentDisplay(ContentDisplay.CENTER);}
                    }

                }

                int finalI = i;
                final String[] sTemp = new String[1];


                roundButton[i].setOnMouseClicked((MouseEvent event) -> {
                    sTemp[0] = "";
                    for(int k=1;k<=LFPC.vertices_length;k++){
                        if(LFPC.graphv[finalI+1][k] !=" ") {
                            sTemp[0] = sTemp[0] + LFPC.vertices[finalI+1] + " -> " + LFPC.graphv[finalI+1][k] + LFPC.vertices[k] + " ; ";
                        } }
                    for(int o=1;o<=LFPC.n;o++) {
                        if (LFPC.vs[o].charAt(0) == LFPC.vertices[finalI + 1] && isLowerCase(LFPC.vs[o].charAt(LFPC.vs[o].length()-2))){
                                sTemp[0] = sTemp[0] + LFPC.vs[o] + " ";
                        }
                        if (LFPC.vs[o].charAt(0) == LFPC.vertices[finalI + 1] && isLowerCase(LFPC.vs[o].charAt(LFPC.vs[o].length()-1))){
                            sTemp[0] = sTemp[0] + LFPC.vs[o] + " ; ";
                        }
                    }


                   vert1Txt.setText(sTemp[0]);

                });

        }
            CubicCurve[] curve = new CubicCurve[LFPC.n-LFPC.nOfEndPoints];
            int k=-1;
            for(int i=1;i<=LFPC.vertices_length;i++) {
                for(int j=1;j<=LFPC.vertices_length;j++) {
                    for(int o=0;o<LFPC.graphv[i][j].length();o++){
                        char c=LFPC.graphv[i][j].charAt(o);
                    if(isLetter(c) ){
                        k++;
                        if (i < j)
                            curve[k] = new CubicCurve(i * 100 + 15, i * 100, i * 100 + 50, i * 100 - 25, j * 100 - 25, j * 100 - 100, j * 100 + 15, j * 100);
                        else if (i > j)
                            curve[k] = new CubicCurve(i * 100 + 15, i * 100 + 30, i * 100 - 100, i * 100 + 20, j * 100, j * 100 + (i - j) * 50, j * 100 + 15, j * 100 + 30);
                        else if (i == j)
                            curve[k] = new CubicCurve(i * 100 + 15, i * 100, i * 100, i * 100 - 30, j * 100 + 30, j * 100 - 30, j * 100 + 20, j * 100 + 5);
                        curve[k].setStroke(Color.BLACK);
                        curve[k].setStrokeWidth(1);
                        curve[k].setFill(null);
                    }
                    }
                }
            }
//            CubicCurve curve1 = new CubicCurve(125, 100, 150, 50, 200, 50, 220, 200);
//            CubicCurve curve2 = new CubicCurve(125, 100, 150, 50, 300, 50, 320, 300);
//            CubicCurve curve3 = new CubicCurve(220, 200, 150, 150, 300, 150, 220, 200);
//            curve1.setStroke(Color.BLACK);
//            curve1.setStrokeWidth(1);
//            curve1.setFill(null);
//            curve2.setStroke(Color.BLACK);
//            curve2.setStrokeWidth(1);
//            curve2.setFill(null);
//            curve3.setStroke(Color.BLACK);
//            curve3.setStrokeWidth(1);
//            curve3.setFill(null);
            Group layout = new Group();



        layout.getChildren().addAll(roundButton);
//            layout.getChildren().add(curve1);
//            layout.getChildren().add(curve2);
//            layout.getChildren().add(curve3);
        layout.getChildren().addAll(curve);
        layout.getChildren().add(vert1Txt);



        Scene scene = new Scene(layout, 1020, 800);

        stage.setTitle("Hello!");
        stage.setScene(scene);
        stage.show();
        }catch (Exception e){System.out.println(e);}
    }

    public static void main(String[] args) {
        LFPC.read();
        LFPC.draw();
        launch();

    }
}