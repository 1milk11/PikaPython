// use crate::class_info;
// use crate::my_string;
#[derive(Debug)]
pub struct ImportInfo {
    pub class_name: String,
    pub import_class_name: String,
}

impl ImportInfo {
    // pub fn new(
    //     class_name: &String,
    //     input_define: String,
    //     file_name: &String,
    // ) -> Option<ImportInfo> {
    //     let define = input_define.replace(" ", "");
    //     let mut import_class_name = match my_string::get_first_token(&define, '(') {
    //         Some(token) => token,
    //         None => return None,
    //     };
    //     import_class_name = match import_class_name.find(".") {
    //         None => class_info::ClassInfo::add_file_profix(&file_name, &import_class_name, false),
    //         Some(_x) => import_class_name.replace(".", "_"),
    //     };
    //     import_class_name = import_class_name.replace(".", "_");
    //     return Some(ImportInfo {
    //         class_name: class_name.clone(),
    //         import_class_name: import_class_name,
    //     });
    // }
}

