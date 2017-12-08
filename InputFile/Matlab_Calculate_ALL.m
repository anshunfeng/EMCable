% 用来测试简单程序的文档，保存在桌面Temp文件夹中，不重要，便于调用测试
% CJPJ85SC 2D 12.3_ThreePhaseSolver_Eigen_results_Asca
% CJPJ85SC 2D 12.3_ThreePhaseSolver_Eigen_results_patch_all_Asca
% CJPJ85SC 2D 12.3_ThreePhaseSolver_Eigen_results_patch_NoAir_Asca
% CJPJ85SC 2D 12.3_ThreePhaseSolver_Eigen_results_patch_NoAir_total_J
% CJPJ85SC 2D 12.3_ThreePhaseSolver_Eigen_results_patch_NoAir_eddy_J

% Use for patch triangle colormap, NOT DELETE
fileForPatch = 'D:\Important Documents\cable_math\EMCable\InputFile\CJPJ85SC 2D 12.3_ThreePhaseSolver_Eigen_results_patch_NoAir_eddy_J.txt';
Array_Patch = load(fileForPatch);
sizeTri = size(Array_Patch, 1)/3;
patch_x = Array_Patch(1:sizeTri, 1:end)';
patch_y = Array_Patch((sizeTri+1):(2*sizeTri), 1:end)';
patch_value = Array_Patch((2*sizeTri+1):3*sizeTri, 1:end)';
patch(patch_x, patch_y,  patch_value)   % patch(patch_x, patch_y,  patch_value, 'EdgeColor', 'k')
colormap((jet))
shading interp % 开不显示网格，关显示网格
colorbar % caxis([  ]); 控制色谱数值范围
view(0,90)
set(gcf,'unit', 'normalized', 'position', [0.2 0.1 0.6 0.8]);

% Use for surf/clabel rectangle colormap, NOT DELETE
figure(2)
fileForSurf = 'D:\Important Documents\cable_math\EMCable\InputFile\CJPJ85SC 2D 12.3_ThreePhaseSolver_Eigen_results_Asca.txt';
Array_Surf = load(fileForSurf);
x = Array_Surf(1:end, 2);
y = Array_Surf(1:end, 3);
z = Array_Surf(1:end, 4);
[X,Y]=meshgrid(linspace(min(x),max(x),100),linspace(min(y),max(y),100)); 
Z = griddata(x,y,z,X,Y,'v4');
Z(Z<0)=0;
[C,h]=contourf(X,Y,Z,10);
clabel(C,h,'LabelSpacing',1000)
clabel(C,h,'fontsize',15,'color','w')


% figure(1)
% [X,Y]=meshgrid(min(x):0.1:max(x),min(y):0.1:max(y));
% Z=griddata(x,y,z,X,Y,'v4');
% surf(X,Y,Z)
% colormap((jet))
% shading interp
% colorbar
% view(0,90)
% 
% ThreePhaseSolver_results_Asca = load('D:\Important Documents\cable_math\EMCable\InputFile\MeshFileForC_ThreePhaseSolver_results_Asca.txt');
% x = ThreePhaseSolver_results_Asca(1:end, 2);
% y = ThreePhaseSolver_results_Asca(1:end, 3);
% z = ThreePhaseSolver_results_Asca(1:end, 4);
% 
% [X,Y]=meshgrid(linspace(min(x),max(x),100),linspace(min(y),max(y),100)); 
% Z = griddata(x,y,z,X,Y,'v4');
% Z(Z<0)=0;
% figure
% [C,h]=contourf(X,Y,Z,10);
% clabel(C,h,'LabelSpacing',1000)
% clabel(C,h,'fontsize',15,'color','w')
% 
% colormap((jet))
% colorbar
% set(gcf,'unit', 'normalized', 'position', [0.2 0.1 0.6 0.8]);
% 
% figure(2)
% x1 = X(50, 50:end);
% z1 = Z(50, 50:end);
% plot(x1, z1)
% set(gcf,'unit', 'normalized', 'position', [0.2 0.1 0.6 0.8]);
% 
% figure(3)
% plot(x2, z2)
% set(gcf,'unit', 'normalized', 'position', [0.2 0.1 0.6 0.8]);