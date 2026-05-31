#include <QApplication>
#include <QPushButton>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

int main(int argc, char *argv[]) {
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (ctx == nullptr) { return -1; }

    if (EVP_PKEY_keygen_init(ctx) <= 0) { return -1; }
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) { return -1; }

    EVP_PKEY *pkey = nullptr;
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) { return -1; }

    FILE *priv_file = fopen("root_private_key.pem", "wb");
    if (priv_file) {
        qDebug() << "???";
        PEM_write_PrivateKey(priv_file, pkey, nullptr, nullptr, 0, nullptr, nullptr);
        fclose(priv_file);
    }
    FILE *pub_file = fopen("root_public_key.pem", "wb");
    if (pub_file) {
        PEM_write_PrivateKey(pub_file, pkey, nullptr, nullptr, 0, nullptr, nullptr);
        fclose(pub_file);
    }

    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}