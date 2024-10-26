# Sistema de Monitoramento e Controle de Rede com Arduino

Este projeto consiste em um sistema de controle automático para monitorar a disponibilidade da rede elétrica e, em caso de falha, acionar um gerador de forma segura e confiável, utilizando a plataforma Arduino. O sistema foi desenvolvido para aplicações em locais onde a continuidade da energia é essencial e utiliza uma estrutura de intertravamento para evitar acidentes, curto-circuitos ou sobrecargas.

## Funcionalidades
- **Monitoramento da rede elétrica e do gerador**: Utiliza sensores para verificar a presença de tensão tanto na rede principal quanto no gerador.
- **Ação automática**: Em caso de falha na rede principal, o sistema libera combustível, inicia o gerador e realiza o chaveamento entre as fontes de energia.
- **Segurança**: Inclui intertravamento para impedir que a rede elétrica e o gerador sejam conectados simultaneamente, evitando curtos e danos ao sistema.
- **Modo manual e automático**: Permite o controle manual para operação de manutenção e configurações, além do modo automático para operação contínua e independente.

## Estrutura do Código
O código é estruturado para operar em duas fases principais:
1. **Fase de Monitoramento**: Detecta a ausência da rede elétrica e aciona o sistema.
2. **Fase de Partida do Gerador**: Libera combustível e dá partida no gerador, repetindo o processo até que a operação seja bem-sucedida ou a rede elétrica retorne.
