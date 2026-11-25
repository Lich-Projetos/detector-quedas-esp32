# Detector de quedas com ESP32

Projeto virtual no Wokwi: https://wokwi.com/projects/478697477338153985

## Estrutura

- `firmware/fall_detector.ino`: sketch para ESP32, MPU6050, buzzer e MQTT.
- `firmware/libraries.txt`: bibliotecas necessárias.
- `wokwi/diagram.json`: montagem para importar no Wokwi.
- `node-red/flow.json`: fluxo para acompanhar as mensagens no painel Debug.

## Configuração

1. Abra o projeto no Wokwi e confira o sketch e as ligações.
2. Instale as bibliotecas listadas em `firmware/libraries.txt`.
3. Antes de executar, use o mesmo tópico MQTT no sketch e no fluxo Node-RED.
4. Importe `node-red/flow.json` no Node-RED e confira o broker e o tópico.
5. Rode a simulação e confirme a mensagem no Debug.

O broker configurado como exemplo é `broker.emqx.io:1883`, sem TLS. Como é um broker público, use um tópico próprio.

O sketch e o diagrama deste repositório são uma referência de trabalho e precisam ser conferidos com a versão original no Wokwi. Não há medições de latência incluídas. O campo `tempo_ms` indica tempo da simulação, não latência.
