//
// Created by bogusz on 26.01.24.
//

#ifndef HUF_H
#define HUF_H

void huf_encoder(const std::filesystem::path &path, const std::string &name,const std::filesystem::path &path_out, const std::string &name_out) {
    std::string p_in = path / (name + ".bin");
    std::string p_out = path_out / (name_out + ".txt");

}

void huf_coder(const std::filesystem::path &path, const std::string &name,const std::filesystem::path &path_out, const std::string &name_out) {
    std::string p_in = path / (name + ".txt");
    std::string p_out = path_out / (name_out + ".bin");

    bsdb::src_provider_impl_file _src(p_in, std::ios::in | std::ios::binary);
    bsdb::src_provider_impl_file _out(p_out, std::ios::out | std::ios::binary);

    _src.ptr_to_end();
    unsigned long size = _src.get_ptr();
    _src.set_ptr(0);
    std::vector<char> tmp;
    tmp.reserve(size);
    char buf;
    for (int i = 0; i < size; ++i) {
        _src.read_obj(buf);
        tmp.push_back(buf);
    }
    std::map<char, int> mapa;
    for (auto in: tmp) {
        mapa[in] += 1;
    }

    std::vector<std::pair<char *, int>> vectorPara(mapa.begin(), mapa.end());
    std::ranges::sort(vectorPara,[](const auto& para1, const auto& para2) {
                  return para1.second < para2.second;
              });

    _out.write_obj(static_cast<unsigned char>(vectorPara.size() - 1));

    if (vectorPara.empty())
        return;

    do {

    } while (vectorPara.size() > 2);

    std::cout << 0;

    // TODO tree Byte | LByte | RByte >> break Byte | {Byte} | {Byte}


}

#endif //HUF_H
