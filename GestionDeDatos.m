%%Gestión de Datos
% Configura los parámetros del canal
readChannelID = 2505791; %
fieldIDs = [1, 2, 3]; % Los campos de PM1.0, PM2.5, y PM10
numDays = 7; % Número de días para visualizar

% Lee los datos de los últimos días
[data, timestamps] = thingSpeakRead(readChannelID, 'Fields', fieldIDs, 'NumDays', numDays, 'OutputFormat', 'Timetable');

% Calcula promedios por hora
dataHourly = retime(data, 'hourly', 'mean');

% Aplicar un filtro de media móvil con una ventana de 3 horas
windowSize = 3; % Tamaño de la ventana para el filtro de media móvil
smoothedData = movmean(dataHourly{:, :}, windowSize);

% Extrae los nuevos datos y tiempos
newTimestamps = dataHourly.Timestamps;

% Umbrales de la OMS para PM1.0, PM2.5, y PM10 (ejemplo en µg/m³)
thresholds = [20, 25, 50]; % 

% Crear la figura
figure;
hold on;

% Colores para cada serie de datos
colors = {'b', 'r', 'g'};

% Nombres para la leyenda
names = {'PM1.0', 'PM2.5', 'PM10'};

% Graficar cada serie de datos
for i = 1:length(fieldIDs)
    plot(newTimestamps, smoothedData(:, i), colors{i}, 'DisplayName', names{i});
    % Añadir línea de umbral
    yline(thresholds(i), 'Color', colors{i}, 'LineStyle', '--', 'DisplayName', sprintf('Umbral %s', names{i}));
end

% Mejorar la visualización
legend show;
title('Promedio Horario de Material Particulado y Umbrales con Filtro de Media Móvil');
xlabel('Fecha y Hora');
ylabel('Concentración Promedio  (µg/m³)');
grid on;

% Destacar áreas donde los umbrales son superados
for i = 1:length(fieldIDs)
    aboveThreshold = smoothedData(:, i) > thresholds(i);
    if any(aboveThreshold)
        % Resaltar datos sobre el umbral
        plot(newTimestamps(aboveThreshold), smoothedData(aboveThreshold, i), 'ko', 'MarkerFaceColor', colors{i});
    end
end

hold off;