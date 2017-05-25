#ifndef _DSM_MESSAGES_H_
#define _DSM_MESSAGES_H_
#include <QMessageBox>
/** Mensagens de warning dos MessageBox Existentes no Sistema */

#ifndef MSG_WARNING_TITLE
#define MSG_WARNING_TITLE           QString::fromUtf8("Aviso")
#endif
#ifndef MSG_WARNING_DELETE_PROFILE
#define MSG_WARNING_DELETE_PROFILE  QString::fromUtf8("Não é possível excluir o perfil pois existem usuários cadastrados com esse perfil.")
#endif
#ifndef MSG_WARNING_DELETE_GROUP
#define MSG_WARNING_DELETE_GROUP    QString::fromUtf8("Não é possível excluir o grupo pois existem produtos cadastrados com esse grupo.")
#endif
#ifndef MSG_WARNING_DELETE_MANUFACTURER
#define MSG_WARNING_DELETE_MANUFACTURER    QString::fromUtf8("Não é possível excluir o fabricante pois existem produtos cadastrados com esse fabricante.")
#endif
#ifndef MSG_WARNING_DELETE_PRODUCT
#define MSG_WARNING_DELETE_PRODUCT  QString::fromUtf8("Não é possível excluir o produto cadastrado.")
#endif
#ifndef MSG_WARNING_DELETE_USER
#define MSG_WARNING_DELETE_USER     QString::fromUtf8("Não é possível excluir o usuário pois existem perfis cadastrados com esse usuário.")
#endif
#ifndef MSG_WARNING_WRONG_PASSWORD
#define MSG_WARNING_WRONG_PASSWORD  QString::fromUtf8("Senha errada! Tente Novamente.")
#endif
#ifndef MSG_WARNING_NEW_PASSWORD
#define MSG_WARNING_NEW_PASSWORD    QString::fromUtf8("Digite sua nova senha !")
#endif
#ifndef MSG_WARNING_DIFFERENTS_PASSWORDS
#define MSG_WARNING_DIFFERENTS_PASSWORDS      QString::fromUtf8("Senhas não conferem!")
#endif
#ifndef MSG_WARNING_USER_EXISTS
#define MSG_WARNING_USER_EXISTS     QString::fromUtf8("O usuário informado já existe!")
#endif
#ifndef MSG_WARNING_ENTRANCE_ALREADY_EXISTS
#define MSG_WARNING_ENTRANCE_ALREADY_EXISTS    QString::fromUtf8("Já existe uma nota fiscal com esse número cadastrado no sistema com o status de %1. A operação não será realizada!")
#endif
#ifndef MSG_INFO_LOGIN_EMPTY
#define MSG_INFO_LOGIN_EMPTY        QString::fromUtf8("Informe um login.")
#endif


/** Mensagens de info dos MessageBox Existentes no Sistema */

#ifndef MSG_INFO_COMBO_MANUFACTURER
#define MSG_INFO_COMBO_MANUFACTURER                 QString::fromUtf8("Selecione um fabricante.")
#endif
#ifndef MSG_INFO_COMBO_GROUP
#define MSG_INFO_COMBO_GROUP                        QString::fromUtf8("Selecione um grupo.")
#endif
#ifndef MSG_INFO_TITLE
#define MSG_INFO_TITLE                              QString::fromUtf8("Informação")
#endif
#ifndef MSG_INFO_RECORD_OK
#define MSG_INFO_RECORD_OK                          QString::fromUtf8("Operação realizada com sucesso.")
#endif
#ifndef MSG_INFO_DELETE_OK
#define MSG_INFO_DELETE_OK                          QString::fromUtf8("Exclusão realizada com sucesso.")
#endif
#ifndef MSG_INFO_NEW_PASSWORD
#define MSG_INFO_NEW_PASSWORD                       QString::fromUtf8("Nova senha cadastrada com sucesso!")
#endif
#ifndef MSG_INFO_NEW_USER
#define MSG_INFO_NEW_USER                           QString::fromUtf8("Usuário cadastrado com sucesso!")
#endif
#ifndef MSG_INFO_EDITED_USER
#define MSG_INFO_EDITED_USER                        QString::fromUtf8("Usuário atualizado com sucesso!")
#endif
#ifndef MSG_INFO_CLIENT_WITHOUT_CREDIT
#define MSG_INFO_CLIENT_WITHOUT_CREDIT              QString::fromUtf8("O cliente informado não possui crédito para essa venda.")
#endif
#ifndef MSG_INFO_CLIENT_WITHOUT_CREDIT2
#define MSG_INFO_CLIENT_WITHOUT_CREDIT2             QString::fromUtf8("O cliente informado não possui crédito.")
#endif

#ifndef MSG_INFO_PRODUCT_WITHOUT_LOTCONTROL
#define MSG_INFO_PRODUCT_WITHOUT_LOTCONTROL         QString::fromUtf8("É necessário associar lotes para esse produto para poder efetuar esta operação.")
#endif

#ifndef MSG_INFO_PRODUCT_WITHOUT_LOTCONTROL_ANV
#define MSG_INFO_PRODUCT_WITHOUT_LOTCONTROL_ANV     QString::fromUtf8("Este produto possui controle da Anvisa e no momento não está com o controle de lote ativado. Para adicionar este produto à entrada é necessário ativar controle de lote.")
#endif

#ifndef MSG_INFO_PRODUCT_TRANSF_WITH_ANVISACONTROL
#define MSG_INFO_PRODUCT_TRANSF_WITH_ANVISACONTROL  QString::fromUtf8("Não é possível realizar transferência de produtos controlado pela Anvisa.")
#endif
#ifndef MSG_INFO_PRODUCT_BALANCE_WITH_ANVISACONTROL
#define MSG_INFO_PRODUCT_BALANCE_WITH_ANVISACONTROL QString::fromUtf8("Não é possível realizar balanço de produtos controlado pela Anvisa.")
#endif
#ifndef MSG_INFO_PRODUCT_EMPTY
#define MSG_INFO_PRODUCT_EMPTY                      QString::fromUtf8("É necessário informar um produto para concluir o movimento.")
#endif
#ifndef MSG_INFO_LOT_EMPTY
#define MSG_INFO_LOT_EMPTY                          QString::fromUtf8("É necessário informar um lote para concluir a operação.")
#endif
#ifndef MSG_INFO_PRODUCT_WITHOUT_AMOUNT
#define MSG_INFO_PRODUCT_WITHOUT_AMOUNT             QString::fromUtf8("Este produto não possui quantidade em estoque.")
#endif
#ifndef MSG_INFO_STORE_EMPTY
#define MSG_INFO_STORE_EMPTY                        QString::fromUtf8("Selecione uma loja de destino antes de finalizar a transferência.")
#endif
#ifndef MSG_INFO_SUPPLIER_EMPTY
#define MSG_INFO_SUPPLIER_EMPTY                     QString::fromUtf8("Selecione um fornecedor antes de finalizar a entrada, deseja selecionar um fornecedor?")
#endif
#ifndef MSG_INFO_TAXDOCNUMBER_EMPTY
#define MSG_INFO_TAXDOCNUMBER_EMPTY                 QString::fromUtf8("Informe o número da nota fiscal.")
#endif
#ifndef MSG_INFO_TAXDOCMODEL_EMPTY
#define MSG_INFO_TAXDOCMODEL_EMPTY                 QString::fromUtf8("Informe o modelo da nota fiscal.")
#endif
#ifndef MSG_INFO_TAXDOCSERIE_EMPTY
#define MSG_INFO_TAXDOCSERIE_EMPTY                 QString::fromUtf8("Informe a série da nota fiscal.")
#endif
#ifndef MSG_INFO_TAXDOCSUBSERIE_EMPTY
#define MSG_INFO_TAXDOCSUBSERIE_EMPTY                 QString::fromUtf8("Informe a subsérie da nota fiscal.")
#endif
#ifndef MSG_INFO_TAXDOCCFOP_EMPTY
#define MSG_INFO_TAXDOCCFOP_EMPTY                 QString::fromUtf8("Informe o CFOP da nota fiscal.")
#endif
#ifndef MSG_INFO_TRANSF_CLOSED_OK
#define MSG_INFO_TRANSF_CLOSED_OK                   QString::fromUtf8("Transferência realizada com sucesso!")
#endif
#ifndef MSG_INFO_TRANSF_EMPTY
#define MSG_INFO_TRANSF_EMPTY                       QString::fromUtf8("Não existe nenhuma transferência cadastrada no sistema.")
#endif
#ifndef MSG_INFO_DESCR_SUPPLIER_EMPTY
#define MSG_INFO_DESCR_SUPPLIER_EMPTY               QString::fromUtf8("É necessário informar o nome do fornecedor para concluir o cadastro.")
#endif
#ifndef MSG_INFO_CNPJ_SUPPLIER_EMPTY
#define MSG_INFO_CNPJ_SUPPLIER_EMPTY                QString::fromUtf8("É necessário informar o cnpj do fornecedor para concluir o cadastro.")
#endif
#ifndef MSG_INFO_SUPPLIER_NOT_EXIST
#define MSG_INFO_SUPPLIER_NOT_EXIST                 QString::fromUtf8("Fornercedor não cadastrado no sistema. Para continuar com a operação é necessário cadastrar o fornecedor. Deseja cadastrar agora?")
#endif
#ifndef MSG_INFO_NFE_CNPJ_INVALID
#define MSG_INFO_NFE_CNPJ_INVALID                   QString::fromUtf8("A operação de importação de NFe detectou um problema. O CNPJ de destino desta Nota Fiscal Eletrônica não confere com o CNPJ dessa empresa, deseja importar mesmo assim?")
#endif
#ifndef MSG_INFO_CANNOT_CONTINUE_NFE
#define MSG_INFO_CANNOT_CONTINUE_NFE                QString::fromUtf8("A operação de importação de NFe não pode prosseguir. O novo produto não foi cadastrado no sistema.")
#endif
#ifndef MSG_INFO_CANNOT_CONTINUE_NFE_SUPPLIER
#define MSG_INFO_CANNOT_CONTINUE_NFE_SUPPLIER       QString::fromUtf8("A operação de importação de NFe não pode prosseguir. O novo fornecedor não foi cadastrado no sistema.")
#endif
#ifndef MSG_INFO_CANNOT_CONTINUE_NFE_BARCODE_EMPTY
#define MSG_INFO_CANNOT_CONTINUE_NFE_BARCODE_EMPTY  QString::fromUtf8("A operação de importação da NFe não poderá prosseguir pois o produto %1 está sem código de barra no arquivo xml. Para proseguir é necessário indicar um código de barra. Deseja informar-lo agora ?")
#endif
#ifndef MSG_INFO_CANNOT_CONTINUE_NFE_NCM_EMPTY
#define MSG_INFO_CANNOT_CONTINUE_NFE_NCM_EMPTY  QString::fromUtf8("A operação de importação de NFe não pode prosseguir. O produto %1 está sem especificação do campo NCM.")
#endif
#ifndef MSG_INFO_CANNOT_CONTINUE_NFE_AMOUNT_ZERO
#define MSG_INFO_CANNOT_CONTINUE_NFE_AMOUNT_ZERO    QString::fromUtf8("A operação de importação de NFe não pode prosseguir. O produto %1 está com quantidade zero no arquivo.")
#endif
#ifndef MSG_INFO_CANNOT_CONTINUE_NFE_COST_ZERO
#define MSG_INFO_CANNOT_CONTINUE_NFE_COST_ZERO      QString::fromUtf8("A operação de importação de NFe não pode prosseguir. O produto %1 está com preço igual a zero no arquivo.")
#endif
#ifndef MSG_INFO_NFEPLUGIN_NOT_FOUND
#define MSG_INFO_NFEPLUGIN_NOT_FOUND                QString::fromUtf8("A operação de importação de NFe não pode prosseguir. Não foi encontrado o plugin para executar esta operação.")
#endif
#ifndef MSG_INFO_PRODUCT_ALREADY_EXISTS
#define MSG_INFO_PRODUCT_ALREADY_EXISTS             QString::fromUtf8("Já existe um produto cadastrado no sistema com esse código de barra.")
#endif
#ifndef MSG_INFO_LOTCONTROL_OK
#define MSG_INFO_LOTCONTROL_OK                      QString::fromUtf8("Não existe nenhuma ocorrência de defasagem de lote no sistema.")
#endif
#ifndef MSG_INFO_INVALID_MS_REGISTER
#define MSG_INFO_INVALID_MS_REGISTER                QString::fromUtf8("Não foi possível salvar o produto. O registro MS não é válido.")
#endif
#ifndef MSG_INFO_COMMISSION_OK
#define MSG_INFO_COMMISSION_OK                      QString::fromUtf8("Baixa de comissão realizada com sucesso!")
#endif
#ifndef MSG_INFO_MOTOBOY_INVALID
#define MSG_INFO_MOTOBOY_INVALID                    QString::fromUtf8("O motoboy informado não existe.")
#endif
#ifndef MSG_INFO_CANNOT_DELETE_ADMIN
#define MSG_INFO_CANNOT_DELETE_ADMIN                QString::fromUtf8("Não é possível excluir o usuário Administrador do sistema.")
#endif
#ifndef MSG_INFO_CANNOT_DELETE_PRODUCT
#define MSG_INFO_CANNOT_DELETE_PRODUCT              QString::fromUtf8("Não é possível excluir produto que possui estoque no sistema.")
#endif
#ifndef MSG_INFO_RESETPASSWORD_SUCCESS
#define MSG_INFO_RESETPASSWORD_SUCCESS              QString::fromUtf8("Sua senha foi recuperada com sucesso! Sua nova senha é <b>12345</b>")
#endif
#ifndef MSG_INFO_PRODUCT_ALREADY_EXIST
#define MSG_INFO_PRODUCT_ALREADY_EXIST              QString::fromUtf8("Já existe este item nesta entrada de nota.")
#endif

/** Mensagens de question dos MessageBox Existentes no Sistema */
#ifndef MSG_QUESTION_TITLE
#define MSG_QUESTION_TITLE                      QString::fromUtf8("Informação")
#endif
#ifndef MSG_QUESTION_PERSIST_PRESCBOOK
#define MSG_QUESTION_PERSIST_PRESCBOOK          QString::fromUtf8("Deseja aproveitar as últimas informações preenchidas?")
#endif
#ifndef MSG_QUESTION_DELETE_PROFILE
#define MSG_QUESTION_DELETE_PROFILE             QString::fromUtf8("Deseja realmente excluir esse perfil?")
#endif
#ifndef MSG_QUESTION_DELETE_STORE
#define MSG_QUESTION_DELETE_STORE               QString::fromUtf8("Deseja realmente excluir essa loja?")
#endif
#ifndef MSG_QUESTION_DELETE_GROUP
#define MSG_QUESTION_DELETE_GROUP               QString::fromUtf8("Deseja realmente excluir esse grupo?")
#endif

#ifndef MSG_QUESTION_MOVE_GROUP
#define MSG_QUESTION_MOVE_GROUP                 QString::fromUtf8("Deseja realmente Mover esse grupo?")
#endif

#ifndef MSG_QUESTION_DELETE_PRODUCT
#define MSG_QUESTION_DELETE_PRODUCT             QString::fromUtf8("Deseja realmente excluir esse produto?")
#endif
#ifndef MSG_QUESTION_DELETE_USER
#define MSG_QUESTION_DELETE_USER                QString::fromUtf8("Deseja realmente excluir esse usuário?")
#endif
#ifndef MSG_QUESTION_NEW_USER
#define MSG_QUESTION_NEW_USER                   QString::fromUtf8("Deseja cadastrar outro usuário?")
#endif

#ifndef MSG_QUESTION_CLOSE
#define MSG_QUESTION_CLOSE                      QString::fromUtf8("Deseja realmente fechar esta tela?")
#endif
#ifndef MSG_QUESTION_FINISH_MOVEMENT
#define MSG_QUESTION_FINISH_MOVEMENT            QString::fromUtf8("Deseja realmente concluir o movimento?")
#endif
#ifndef MSG_QUESTION_INFO_OK
#define MSG_QUESTION_INFO_OK                    QString::fromUtf8("As informações estão corretas?")
#endif
#ifndef MSG_QUESTION_CANCEL_EDIT
#define MSG_QUESTION_CANCEL_EDIT                QString::fromUtf8("Deseja realmente cancelar a edição?")
#endif
#ifndef MSG_QUESTION_CANCEL_TRANSF
#define MSG_QUESTION_CANCEL_TRANSF              QString::fromUtf8("Deseja realmente cancelar a transferência?")
#endif
#ifndef MSG_QUESTION_CHECK_ACCEPT
#define MSG_QUESTION_CHECK_ACCEPT               QString::fromUtf8("Deseja realmente confirmar a aceitação desse envio?")
#endif
#ifndef MSG_QUESTION_CLOSE_COMMISSION
#define MSG_QUESTION_CLOSE_COMMISSION           QString::fromUtf8("Deseja realmente realizar a baixa das comissões entre os dias %1 e %2?")
#endif
#ifndef MSG_QUESTION_PRODUCT_WITHOUT_LOTCONTROL
#define MSG_QUESTION_PRODUCT_WITHOUT_LOTCONTROL QString::fromUtf8("O Produto selecionado não esta marcado como produto controlado por lote. Você deseja habilitar este controle?")
#endif
#ifndef MSG_QUESTION_RESETPASSWORD
#define MSG_QUESTION_RESETPASSWORD              QString::fromUtf8("Deseja realmente recuperar sua senha ?")
#endif

/** Suporte Técnico */
#ifndef MSG_SUPPORT
#define MSG_SUPPORT                 "\nPor favor, entre em contato com o nosso suporte técnico: +55 (92) 3213-6619."
#endif

/** Mensagens de error dos MessageBox Existentes no Sistema */
#ifndef MSG_ERROR_TITLE
#define MSG_ERROR_TITLE                     QString::fromUtf8("Erro no Sistema")
#endif
#ifndef MSG_ERROR_RECORD_TITLE
#define MSG_ERROR_RECORD_TITLE              QString::fromUtf8("Erro no Cadastro")
#endif
#ifndef MSG_ERROR_DATABASE_TITLE
#define MSG_ERROR_DATABASE_TITLE            QString::fromUtf8("Erro no Banco de Dados")
#endif
#ifndef MSG_ERROR_DATABASE
#define MSG_ERROR_DATABASE                  QString::fromUtf8("Não foi possível persistir a informação no banco de dados.")
#endif

#ifndef MSG_ERROR_CONNECT_DB
#define MSG_ERROR_CONNECT_DB                QString::fromUtf8("Não foi possível estabelecer conexão com o Banco de Dados.")
#endif

#ifndef MSG_ERROR_MOVEMENT
#define MSG_ERROR_MOVEMENT                  QString::fromUtf8("Não foi possível concluir o movimento.")
#endif

#ifndef MSG_ERROR_BALANCE
#define MSG_ERROR_BALANCE                   QString::fromUtf8("Não foi possível concluir o balanço.")
#endif
#ifndef MSG_ERROR_INSERT_TRANSF_ITEM
#define MSG_ERROR_INSERT_TRANSF_ITEM        QString::fromUtf8("Erro ao persistir os itens da transferência no banco de dados.")
#endif
#ifndef MSG_ERROR_INSERT_TRANSF_ITEM_LOT
#define MSG_ERROR_INSERT_TRANSF_ITEM_LOT    QString::fromUtf8("Erro ao persistir o controle de lote dos itens da transferência no banco de dados.")
#endif
#ifndef MSG_ERROR_CANCEL_TRANSF_ITEM
#define MSG_ERROR_CANCEL_TRANSF_ITEM        QString::fromUtf8("Erro ao cancelar os itens da transferência no banco de dados.")
#endif
#ifndef MSG_ERROR_CANCEL_TRANSF_ITEM_LOT
#define MSG_ERROR_CANCEL_TRANSF_ITEM_LOT    QString::fromUtf8("Erro ao cancelar o controle de lote dos itens da transferência no banco de dados.")
#endif
#ifndef MSG_ERROR_INSERT_ENTRANCE_ITEM_LOT
#define MSG_ERROR_INSERT_ENTRANCE_ITEM_LOT  QString::fromUtf8("Erro ao persistir o controle de lote dos itens da entrada no banco de dados.")
#endif
#ifndef MSG_ERROR_UPDATE_GAIN
#define MSG_ERROR_UPDATE_GAIN               QString::fromUtf8("Falha ao atualizar a margem de lucro dos produtos.")
#endif
#ifndef MSG_ERROR_UPDATE_PRICE
#define MSG_ERROR_UPDATE_PRICE              QString::fromUtf8("Falha ao atualizar o preço de venda dos produtos.")
#endif
#ifndef MSG_ERROR_UPDATE_MAX_DISCOUNT
#define MSG_ERROR_UPDATE_MAX_DISCOUNT       QString::fromUtf8("Falha ao atualizar desconto máximo dos produtos.")
#endif
#ifndef MSG_ERROR_UPDATE_PROM_DISCOUNT
#define MSG_ERROR_UPDATE_PROM_DISCOUNT      QString::fromUtf8("Falha ao atualizar desconto promocional dos produtos.")
#endif
#ifndef MSG_ERROR_UPDATE_COMMISSION
#define MSG_ERROR_UPDATE_COMMISSION         QString::fromUtf8("Falha ao atualizar comissão dos produtos.")
#endif
#ifndef MSG_ERROR_UPDATE_STOCK
#define MSG_ERROR_UPDATE_STOCK              QString::fromUtf8("Falha ao atualizar estoque dos produtos.")
#endif
#ifndef MSG_ERROR_UPDATE_SOLICITATION
#define MSG_ERROR_UPDATE_SOLICITATION       QString::fromUtf8("Falha ao atualizar solicitação de estoque dos produtos.")
#endif
#ifndef MSG_ERROR_UPDATE_ICMS
#define MSG_ERROR_UPDATE_ICMS               QString::fromUtf8("Falha ao atualizar aliquota de ICMS dos produtos.")
#endif
#ifndef MSG_ERROR_UPDATE_TRIB
#define MSG_ERROR_UPDATE_TRIB               QString::fromUtf8("Falha ao atualizar situação tributária dos produtos.")
#endif
#ifndef MSG_ERROR_UPDATE_RESERVE_STOCK
#define MSG_ERROR_UPDATE_RESERVE_STOCK      QString::fromUtf8("Falha ao atualizar reserva de estoque dos produtos.")
#endif
#ifndef MSG_ERROR_CLOSE_COMMISSION
#define MSG_ERROR_CLOSE_COMMISSION          QString::fromUtf8("Falha ao fechar as comissões.")
#endif
#ifndef MSG_ERROR_INSERT_COMMISSION_HISTORY
#define MSG_ERROR_INSERT_COMMISSION_HISTORY QString::fromUtf8("Falha ao persistir histórico de comissões no banco de dados.")
#endif

/** Mensagens da Tela de Sngpc */
#ifndef MSG_INFO_START_DATE
#define MSG_INFO_START_DATE                     QString::fromUtf8("A data inicial não pode ser inferior a 7 dias da data atual.")
#endif
#ifndef MSG_INFO_END_DATE_CONFLICT
#define MSG_INFO_END_DATE_CONFLICT              QString::fromUtf8("A data final deve obtigatoriamente ser inferior a data de hoje.")
#endif
#ifndef MSG_INFO_END_DATE
#define MSG_INFO_END_DATE                       QString::fromUtf8("A data inicial não pode ser inferior a última data da geração do arquivo.")
#endif
#ifndef MSG_ERROR_CREATE_FILE
#define MSG_ERROR_CREATE_FILE                   QString::fromUtf8("Não foi possível criar o arquivo %1.")
#endif
#ifndef MSG_INFO_GENERATED_SUCESS_XML_FILE
#define MSG_INFO_GENERATED_SUCESS_XML_FILE      QString::fromUtf8("O arquivo foi gerado com sucesso.")
#endif
#ifndef MSG_INFO_DIR_NOT_EXISTS
#define MSG_INFO_DIR_NOT_EXISTS                 QString::fromUtf8("Não é possível gerar o arquivo pois o diretório base informado não existe. Vá em Sngpc->Configurações na tela principal do sistema para não informar um diretório base válido.")
#endif
#ifndef MSG_INFO_LAST_DATE_IS_TODAY
#define MSG_INFO_LAST_DATE_IS_TODAY             QString::fromUtf8("Hoje já foi gerado o arquivo de produtos controlados.")
#endif
#ifndef MSG_LAST_DATE
#define MSG_LAST_DATE                           QString::fromUtf8("O último arquivo gerado foi do dia %1.")
#endif
#ifndef MSG_NOT_EXIST_DATE
#define MSG_NOT_EXIST_DATE                      QString::fromUtf8("Não foi gerado nenhum arquivo até o momento.")
#endif
#ifndef MSG_ERROR_DATE_IN_FUTURE
#define MSG_ERROR_DATE_IN_FUTURE                QString::fromUtf8("A data informada está no futuro.")
#endif


/** Mensagens da Tela de Atualização de Preços ABCFarma */
#ifndef MSG_SUCESS_ABCFARMA_CONVERSION
#define MSG_SUCESS_ABCFARMA_CONVERSION      QString::fromUtf8("Atualização efetuada com sucesso.")
#endif

#ifndef MSG_NOCOMTIBLE_ABCFARMA_FILE
#define MSG_NOCOMTIBLE_ABCFARMA_FILE        QString::fromUtf8("O arquivo informado não possui o padrão da ABCFarma\n\nAtenção: O arquivo informado deve conter todas as alíquotas e\no registro dos medicamentos junto ao ministério da saúde.")
#endif

#ifndef MSG_ERROR_NOFILEPATH
#define MSG_ERROR_NOFILEPATH                QString::fromUtf8("Você precisa informar o arquivo TABELA.TXT da \nABCFarma para atualização.")
#endif

#ifndef MSG_ALREADY_UPDATED
#define MSG_ALREADY_UPDATED                 QString::fromUtf8("O sistema já foi atualizado apartir do arquivo informado.\nVocê desejar atualizar novamente ?")
#endif

#ifndef MSG_CANNOT_OPENFILE
#define MSG_CANNOT_OPENFILE                 QString::fromUtf8("O sistema não conseguiu abrir o arquivo!\nO arquivo informado não existe ou está corrompido.")
#endif


/** Mensagens da Tela de Produtos */
#ifndef MSG_QUESTION_CANCEL_NEW
#define MSG_QUESTION_CANCEL_NEW         QString::fromUtf8("Você tem certeza que deseja cancelar o cadastro do novo produto?")
#endif
#ifndef MSG_INFO_PRODUCT_UNKNOW
#define MSG_INFO_PRODUCT_UNKNOW         QString::fromUtf8("Você precisa selecionar o produto para remoção!")
#endif

#ifndef MSG_ERROR_CREATE_TEMP_TABLE_INFORMATIVE
#define MSG_ERROR_CREATE_TEMP_TABLE_INFORMATIVE QString::fromUtf8("O sistema encontrou problemas ao tentar abrir a tela de pré-vendas!")
#endif
#ifndef MSG_ERROR_CREATE_TEMP_TABLE
#define MSG_ERROR_CREATE_TEMP_TABLE             QString::fromUtf8("Não foi possível criar tabela temporária. Soluções que podem ajudar a resolver o problema:\n" \
		"1. Problema de permissão para criação de tabela temporária junto ao banco de dados.\n" \
		"2. O serial do seu HD contem caracteres considerados inválidos.\n" \
		"Error retornado pelo SGBD:\n")
#endif
#ifndef MSG_INFO_PRODUCT_WITHOUT_LOTCONTROL_ID
#define MSG_INFO_PRODUCT_WITHOUT_LOTCONTROL_ID  QString::fromUtf8("Ainda falta indicar o lote para o produto\n%1.")
#endif
#ifndef MSG_INFO_PRODUCT_WITH_LOTCONTROL
#define MSG_INFO_PRODUCT_WITH_LOTCONTROL        QString::fromUtf8("Este produto já está com o(s) lote(s) indicado(s).")
#endif

/** Mensagens relacioandas a licenca*/
#ifndef MSG_ATTENTION_TITLE
#define MSG_ATTENTION_TITLE             QString::fromUtf8("Atenção")
#endif
#ifndef MSG_FAULT_TITLE
#define MSG_FAULT_TITLE                 QString::fromUtf8("FALHA GRAVE: Seu sistema pode estar em risco")
#endif
#ifndef MSG_ERROR_LICENCE_INVALID
#define MSG_ERROR_LICENCE_INVALID       QString::fromUtf8("A licença informada não é válida. Portanto, a funcionalidade de Relatórios ficará indisponível. Para resolver este problema, entre em contato com a Lnx Solutions.")
#endif
#ifndef MSG_INFO_LICENCE_EMPTY
#define MSG_INFO_LICENCE_EMPTY          QString::fromUtf8("Informe uma licença válida.")
#endif
#ifndef MSG_INFO_LICENCE_VALID
#define MSG_INFO_LICENCE_VALID          QString::fromUtf8("Sua licença foi atualizada com sucesso.")
#endif
#ifndef MSG_ERROR_DATE_FRAUD
#define MSG_ERROR_DATE_FRAUD            QString::fromUtf8("Foi detectada uma inconsistência de datas no sistema. Por favor, atualize a data desta estação de trabalho. Caso o problema persista entre em contato com a Lnx Solutions.")
#endif

/** Mensagens da Tela de Erro Conexão com Banco de Dados **/
#ifndef MSG_ERROR_DB_CONNECTION_TEXT
#define MSG_ERROR_DB_CONNECTION_TEXT            QString::fromUtf8("Oops... Não foi possível conectar ao servidor!")
#endif
#ifndef MSG_ERROR_DB_CONNECTION_INFORMATIVE
#define MSG_ERROR_DB_CONNECTION_INFORMATIVE     QString::fromUtf8("Pode ser que exista algum problema de comunicação na rede ou o sistema não foi configurado corretamente!")
#endif
#ifndef MSG_ERROR_DB_CONNECTION_TITLE
#define MSG_ERROR_DB_CONNECTION_TITLE           QString::fromUtf8("Error de conexão com servidor...")
#endif

#ifndef STATUS_CHECKPRODUCTS_QTD_LESS
#define STATUS_CHECKPRODUCTS_QTD_LESS           QString::fromUtf8("ITEM FALTANTE")
#endif

#ifndef STATUS_CHECKPRODUCTS_QTD_OVER
#define STATUS_CHECKPRODUCTS_QTD_OVER           QString::fromUtf8("ITEM EXTRA")
#endif

#ifndef STATUS_CHECKPRODUCTS_COST_LESS
#define STATUS_CHECKPRODUCTS_COST_LESS           QString::fromUtf8("CUSTO MENOR")
#endif

#ifndef STATUS_CHECKPRODUCTS_COST_OVER
#define STATUS_CHECKPRODUCTS_COST_OVER           QString::fromUtf8("CUSTO MAIOR")
#endif

#ifndef STATUS_CHECKPRODUCTS_OVER_PRODUCTS
#define STATUS_CHECKPRODUCTS_OVER_PRODUCTS    QString::fromUtf8("PRODUTO EXTRA")
#endif

#ifndef STATUS_CHECKPRODUCTS_LESS_PRODUCTS
#define STATUS_CHECKPRODUCTS_LESS_PRODUCTS    QString::fromUtf8("PRODUTO FALTANTE")
#endif

#define MSG_INFO_CANNOT_EDIT_ACCOUNTTYPE    QString::fromUtf8("Não é possível editar esse tipo de conta do sistema.")

#define MSG_INFO_CANNOT_DELETE_ACCOUNTTYPE  QString::fromUtf8("Não é possível excluir esse tipo de conta do sistema.")

#define MSG_QUESTION_DELETE_ACCOUNTTYPE     QString::fromUtf8("Deseja realmente excluir esse tipo de conta?")

#define MSG_QUESTION_RESTORE_ACCOUNT        QString::fromUtf8("Deseja realmente estornar essa conta?")

#define MSG_QUESTION_DELETE_ACCOUNTTOPAY    QString::fromUtf8("Deseja realmente excluir essa conta?")

#define MSG_ERROR_SAVE_ACCOUNT              QString::fromUtf8("Não foi possível persistir as informações de conta no sistema.")

#define MSG_QUESTION_DELETE_BANK            QString::fromUtf8("Deseja realmente excluir esse banco?")

#define MSG_INFO_BANK_ALREADY_EXIST         QString::fromUtf8("O código desse banco já existe no sistema.")

#define MSG_QUESTION_ACCOUNT_ZERO           QString::fromUtf8("Informe um valor maior que zero.")
#endif // DSM_MESSAGES_H
