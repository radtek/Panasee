# declare directories containing used libraries
LINK_DIRECTORIES(${dcmsign_BINARY_DIR} ${ofstd_BINARY_DIR} ${dcmdata_BINARY_DIR} ${ZLIB_LIBDIR} ${OPENSSL_LIBDIR})

# declare executables
ADD_EXECUTABLE(dcmsign dcmsign)

# declare installation files
INSTALL_TARGETS(${INSTALL_BINDIR} dcmsign)

# make sure executables are linked to the corresponding libraries
TARGET_LINK_LIBRARIES(dcmsign netapi32 wsock32 ofstd dcmdata dcmdsig ${ZLIB_LIBS} ${OPENSSL_LIBS})
