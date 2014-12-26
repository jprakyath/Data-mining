clear all;

%Load data from saved training.mat file
load('training.mat');
load('test.mat');

string_variables = {'AUCGUART' 'Auction' 'Color' 'Make' 'Model' 'Nationality'...
    'PRIMEUNIT' 'Size' 'SubModel' 'TopThreeAmericanName' 'Transmission' ...
    'Trim' 'VNST' 'WheelType'}; 
categorical_variables = {'BYRNO' 'VNZIP' 'WheelTypeID'};
numerical_variables = {'MMRAcquisitionAuctionAver', 'MMRAcquisitionAuctionClea', ...
    'MMRAcquisitionRetailAvera', 'MMRAcquisitonRetailCleanP', 'MMRCurrentAuctionAverageP', ...
    'MMRCurrentAuctionCleanPri', 'MMRCurrentRetailAveragePr', 'MMRCurrentRetailCleanPric', ...
    'VehBCost', 'VehOdo', 'VehYear', 'VehicleAge', 'WarrantyCost', 'PurchDate'};
changed_variable_names = {'IsBadBuy', 'PurchDate', 'Auction', 'VehYear', ...
    'VehicleAge', 'Make', 'Model', 'Trim', 'SubModel', 'Color', ...
    'Transmission', 'WheelTypeID', 'WheelType', 'VehOdo', ...
    'Nationality', 'Size', 'TopThreeAmericanName', 'MMRAcquisitionAuctionAver', ...
    'MMRAcquisitionAuctionClea', 'MMRAcquisitionRetailAvera', ...
    'MMRAcquisitonRetailCleanP', 'MMRCurrentAuctionAverageP', ...
    'MMRCurrentAuctionCleanPri', 'MMRCurrentRetailAveragePr', ...
    'MMRCurrentRetailCleanPric', 'PRIMEUNIT', 'AUCGUART', ...
    'BYRNO', 'VNZIP', 'VNST', 'VehBCost', 'IsOnlineSale', 'WarrantyCost'};

%Seperate different kinds of variables
VNZIP = VNZIP1;
VNZIP1 = VNZIP2;

%Categorical variables
for i=1:length(string_variables)
   expression = ['keys = unique(' string_variables{i} ');']; 
   eval(expression); 
   %Training Vars
   expression = ['[lia, locb] = ismember(' string_variables{i} ', keys);']; 
   eval(expression);
   %Convert string into integer label  
   expression = [string_variables{i} '= locb;'];
   eval(expression)
   %Test Vars
   expression = ['[lia, locb] = ismember(' string_variables{i} '1, keys);']; 
   eval(expression);
   %Convert string into integer label  
   expression = [string_variables{i} '1= locb;'];
   eval(expression)
end

%Replace NaN with -1
for i=1:length(categorical_variables)
    %Training data
    expression = [categorical_variables{i} '(isnan(' categorical_variables{i} '))= -1;'];
    eval(expression);
    %Test data
    expression = [categorical_variables{i} '1(isnan(' categorical_variables{i} '1))= -1;'];
    eval(expression);
end

%Convert the string dates into continous dates
PurchDate = datenum(PurchDate, 'mm/dd/yy');
PurchDate1 = datenum(PurchDate1, 'mm/dd/yy');

%Numerical Variables
for i=1:length(numerical_variables)
    %Seperate missing values
    expression = ['non_nan=' numerical_variables{i} '(~isnan(' numerical_variables{i} '));'];
    eval(expression);   
    expression = ['non_nan_class= IsBadBuy(~isnan(' numerical_variables{i} '));'];
    eval(expression);   
    expression = ['non_nan_test=' numerical_variables{i} '1(~isnan(' numerical_variables{i} '1));'];
    eval(expression);

    %If fewer than 25 unique values, don't discretize
    [uniqueVal, ind_unique, ind_non_nan] = unique(non_nan);
    if size(uniqueVal,1)>25
        %Divide the data into quantiles
        bins = quantile(non_nan, 4);
        bins = [min(non_nan), bins, max(non_nan)];
        %Training data
        [counts, ind]=histc(non_nan, bins);
        expression = [numerical_variables{i} '(~isnan(' numerical_variables{i} '))= ind;'];
        eval(expression);
        %Test data
        [counts, ind]=histc(non_nan_test, bins);
        expression = [numerical_variables{i} '1(~isnan(' numerical_variables{i} '1))= ind;'];
        eval(expression);
    else 
        %Training data
        expression = [numerical_variables{i} '(~isnan(' numerical_variables{i} '))= ind_non_nan;'];
        eval(expression);
        %Test data
        [lia, locb] = ismember( non_nan_test, uniqueVal); 
        expression = [numerical_variables{i} '1(~isnan(' numerical_variables{i} '1))= locb;'];
        eval(expression);
    end
    
    %NaN values are in bin -1
    %Training data
    expression = [numerical_variables{i} '(isnan(' numerical_variables{i} '))= -1;'];
    eval(expression);
    %Test data
    expression = [numerical_variables{i} '1(isnan(' numerical_variables{i} '1))= -1;'];
    eval(expression);
end

Attribute_names = [IsBadBuy, PurchDate, Auction, VehYear, VehicleAge, Make, Model, ...
    Trim, SubModel, Color, Transmission, WheelTypeID, WheelType, VehOdo, ...
    Nationality, Size, TopThreeAmericanName, MMRAcquisitionAuctionAver, ...
    MMRAcquisitionAuctionClea, MMRAcquisitionRetailAvera, ...
    MMRAcquisitonRetailCleanP, MMRCurrentAuctionAverageP, ...
    MMRCurrentAuctionCleanPri, MMRCurrentRetailAveragePr, ...
    MMRCurrentRetailCleanPric, PRIMEUNIT, AUCGUART, ...
    BYRNO, VNZIP, VNST, VehBCost, IsOnlineSale, WarrantyCost];

cell2csv('training_preprocessed.csv', [changed_variable_names; mat2cell(Attribute_names, ones(size(Attribute_names, 1),1), ...
    ones(size(Attribute_names, 2),1))], ',');

Attribute_names = [PurchDate1, Auction1, VehYear1, VehicleAge1, Make1, Model1, ...
    Trim1, SubModel1, Color1, Transmission1, WheelTypeID1, WheelType1, VehOdo1, ...
    Nationality1, Size1, TopThreeAmericanName1, MMRAcquisitionAuctionAver1, ...
    MMRAcquisitionAuctionClea1, MMRAcquisitionRetailAvera1, ...
    MMRAcquisitonRetailCleanP1, MMRCurrentAuctionAverageP1, ...
    MMRCurrentAuctionCleanPri1, MMRCurrentRetailAveragePr1, ...
    MMRCurrentRetailCleanPric1, PRIMEUNIT1, AUCGUART1, ...
    BYRNO1, VNZIP1, VNST1, VehBCost1, IsOnlineSale1, WarrantyCost1];

cell2csv('test_preprocessed.csv', [changed_variable_names(2:end); mat2cell(Attribute_names, ones(size(Attribute_names, 1),1), ...
    ones(size(Attribute_names, 2),1))], ',');
