%%Generación Alertas
%%Configura los parámetros del canal
readChannelID = 2505791; % Reemplaza con el ID de tu canal
readAPIKey = 'BV8UNHXBMGML733H'; % Reemplaza con tu API Key de lectura

% Lee los últimos datos de PM1.0, PM2.5, y PM10
[data, timestamps] = thingSpeakRead(readChannelID, 'Fields', [1, 2,3], 'NumPoints', 1, 'ReadKey', readAPIKey);

% Umbrales de la OMS para PM1.0, PM2.5, y PM10 (ejemplo en µg/m³)
thresholds = [30, 25, 40]; 

% Comprueba si algún valor excede los umbrales
exceeds = data > thresholds;

if any(exceeds)
    % Construye un mensaje de alerta
    alertMessage = sprintf('Alerta: Niveles altos detectados a %s. PM1.0: %d, PM2.5: %d, PM10: %d.', datestr(timestamps), data(1), data(2), data(3));
    
    % Enviar alerta por correo electrónico o Twitter (configurar en ThingSpeak React)
    % Aquí podrías usar una función como sendmail (configura previamente en MATLAB) o directamente usar ThingSpeak para enviar una alerta.
    fprintf(alertMessage); % Solo para propósitos de prueba
else
    fprintf('Todos los niveles están dentro de los límites seguros.'); % Solo para propósitos de prueba
end