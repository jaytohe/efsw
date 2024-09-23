#include <efsw/efsw.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

class PyFileWatchListener : public efsw::FileWatchListener {
public:
    /* Inherit the constructors */
    virtual ~PyFileWatchListener() {}

    /* Trampoline (need one for each virtual function) */
    void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename) {
        PYBIND11_OVERRIDE_PURE(
            void, /* Return type */
            efsw::FileWatchListener, /* Parent class */
            handleFileAction, /* Name of function in C++ (must match Python name) */
	    /* Argument(s) */
            watchid,
            dir,
            filename,
            action,
            oldFilename
        );
    }
};

/*
‘efsw::WatchID efsw::FileWatcher::addWatch(const string&, efsw::FileWatchListener*, bool)’

*/

PYBIND11_MODULE(pyefsw, m) {
	py::class_<efsw::FileWatcher>(m, "FileWatcher")
		.def(py::init<>())
		.def(py::init<bool>(), py::arg("useGenericFileWatcher")) // Only allow initialization with explicit constructor FileWatcher( bool useGenericFileWatcher );
		.def("addWatch", static_cast<efsw::WatchID (efsw::FileWatcher::*)(const std::string&, efsw::FileWatchListener*)>(&efsw::FileWatcher::addWatch))
		.def("addWatch", static_cast<efsw::WatchID (efsw::FileWatcher::*)(const std::string&, efsw::FileWatchListener*, bool)>(&efsw::FileWatcher::addWatch))
		.def("addWatch", static_cast<efsw::WatchID (efsw::FileWatcher::*)(const std::string&, efsw::FileWatchListener*, bool, const std::vector<efsw::WatcherOption>&)>(&efsw::FileWatcher::addWatch))
		.def("watch", &efsw::FileWatcher::watch)
		.def("removeWatch", static_cast<void (efsw::FileWatcher::*)(const std::string&)>(&efsw::FileWatcher::removeWatch))
		.def("removeWatch", static_cast<void (efsw::FileWatcher::*)(efsw::WatchID)>(&efsw::FileWatcher::removeWatch))
		.def("directories", &efsw::FileWatcher::directories)
		.def("followSymlinks", static_cast<void (efsw::FileWatcher::*)(bool)>(&efsw::FileWatcher::followSymlinks))
		.def("isFollowingSymlinks", static_cast<const bool& (efsw::FileWatcher::*)() const>(&efsw::FileWatcher::followSymlinks))
		.def("allowOutOfScopeLinks", static_cast<void (efsw::FileWatcher::*)(bool)>(&efsw::FileWatcher::allowOutOfScopeLinks))
       .def("isAllowingOutOfScopeLinks", static_cast<const bool& (efsw::FileWatcher::*)() const>(&efsw::FileWatcher::allowOutOfScopeLinks));

	// Specify trampoline class for FileWatchListener which handles control back to Python (so we can implement the FileWatchListener interface on the Python side)
	py::class_<efsw::FileWatchListener, PyFileWatchListener>(m, "FileWatchListener")
		.def(py::init<>())
		.def("handleFileAction", &efsw::FileWatchListener::handleFileAction);

	//Make the Action enum available to Python side
	py::enum_<efsw::Action>(m, "Action")
		.value("Add", efsw::Action::Add)
		.value("Delete", efsw::Action::Delete)
		.value("Modified", efsw::Action::Modified)
		.value("Moved", efsw::Action::Moved)
		.export_values();

	py::class_<efsw::WatcherOption>(m, "WatcherOption")
		.def(py::init<efsw::Option, int>(), py::arg("option"), py::arg("value"))
		.def_readwrite("option", &efsw::WatcherOption::mOption)
		.def_readwrite("value", &efsw::WatcherOption::mValue);

	// Make the Option enum available to Python side
	py::enum_<efsw::Option>(m, "Option")
		.value("WinBufferSize", efsw::Option::WinBufferSize)
		.value("WinNotifyFilter", efsw::Option::WinNotifyFilter)
		.export_values();

}