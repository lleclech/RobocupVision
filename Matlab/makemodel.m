%function [save] = makemodel()

close all;
clear all;

%Extraction de la premi�re image
[A,map,alpha] = imread('../data/reference/terrain117.png');
if isempty(A)
    return;
end
%A = s.cdata;

%figure, imshow(A)


%Ici je cr�e une matrice qui r�cup�re les points non blancs dans l'image
C = ones(size(A));
for i = 1:size(A,1)
    for j = 1:size(A,2)
        for k = 1:size(A,3)
            if (A(i,j,k) ~= 255)
                C(i,j,k) = A(i,j,k);
            else
                C(i,j,k) = 0;
            end
        end
    end
end
C;

%C= imcrop(A);
%figure, imagesc(C)



%composantes RGB (pour C)
Rp =C (:,:,1);% prend le premier element
Gp =C (:,:,2);
Bp =C (:,:,3);
Rp =Rp(1:end);%transforme en vecteur
Gp =Gp(1:end);
Bp =Bp(1:end);


% Apr�s avoir r�cup�r� les composantes RGB, je transforme mes r�sultats
% pour supprimer tous les �l�ments superflu de notre modele (ici les 0)

m=0;
n=0;
p=0;

for a= 1:size(Rp,2)
    if (Rp(a) ~= 0)
       m = m + 1;
       R(m) = Rp(a);
    end
end

for b= 1:size(Gp,2)
    if (Gp(b) ~= 0)
       n = n + 1;
       G(n) = Gp(b);
    end
end

for c= 1:size(Bp,2)
    if (Bp(c) ~= 0)
       p = p + 1;
       B(p) = Bp(c);
    end
end

[T,map2,alpha2] = imread('rgb46.png');

%composantes RGB (pour l'autre image)
R2 =T (:,:,1);% prend le premier element
G2 =T (:,:,2);
B2 =T (:,:,3);
R2 =R2(1:end);%transforme en vecteur
G2 =G2(1:end);
B2 =B2(1:end);

%attribution des composantes 1 et 2
C1 = R;
C2 = G;
C = [C1;C2];
C1image = R2;
C2image = G2;
Cimg = [C1image;C2image];

%Affiche les points
%figure, plot(C1,C2,'.')

%barycentre
baryC1 = mean(C1);
baryC2 = mean(C2);
Bary = [baryC1;baryC2];

%variances-covariances
varC1 = sum((C1-baryC1).^2)/size(C1,2);
varC2 = sum((C2-baryC2).^2)/size(C2,2);
covarC1C2 = sum((C1-baryC1).*(C2-baryC2))/size(C1,2);
Mcov = [varC1 covarC1C2; covarC1C2 varC2];


%Calcul de la distance
%newBary1 = ones(1,size(C,2))*baryC1;
%newBary2 
%D = (C - Bary).*(inv(Mcov))*(C - Bary)

%Converti l'image en double
C = double(C);
Cimg = double(Cimg);

%matrice r�sultat
D = zeros(size(C,2),1);
Dimage = zeros(size(Cimg,2),1);

%Distance pour la region cibl�e
for i=1:size(C,2)
    D(i,1) = (C(:,i)-Bary)'*(inv(Mcov))*(C(:,i) - Bary);
end;
%distance pour l'image totale
for i=1:size(Cimg,2)
    Dimage(i,1) = (Cimg(:,i)-Bary)'*(inv(Mcov))*(Cimg(:,i) - Bary);
end;

%determination du seuil
seuil = 1.9;
%seuilmax = max(D)/10;

%cr�e l'image binaire
imgBin = zeros(size(A,1),size(A,2));
for j=1:size(A,2)
    for i=1:size(A,1)
        k = (j-1)*size(A,1)+i;
        if (Dimage(k)>=seuil)
            imgBin(i,j)=1;
        end;
    end;
end;
figure, imshow(imgBin)

%sauvegarde
save('newterrain.txt', 'Mcov', 'Bary', '-ascii');