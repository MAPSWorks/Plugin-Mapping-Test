#ifndef CAMERASMODEL_H
#define CAMERASMODEL_H

#include "dataitemsmodel.h"

class QCameraData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(qint32 focusMM READ focusMM WRITE setFocusMM NOTIFY focusMMChanged)
    Q_PROPERTY(qreal lxMM READ lxMM WRITE setLxMM NOTIFY lxMMChanged)
    Q_PROPERTY(qreal lyMM READ lyMM WRITE setLyMM NOTIFY lyMMChanged)
    Q_PROPERTY(qint32 ax READ ax WRITE setAx NOTIFY axChanged)
    Q_PROPERTY(qint32 ay READ ay WRITE setAy NOTIFY ayChanged)

public:
    QCameraData(QObject *parent = nullptr) : QObject(parent) {}
    QCameraData(const QString &name, qint32 focusMM, qreal lxMM, qreal lyMM, qint32 ax, qint32 ay, QObject *parent)
        : QObject(parent)
        , name_(name)
        , focusMM_(focusMM)
        , lxMM_(lxMM)
        , lyMM_(lyMM)
        , ax_(ax)
        , ay_(ay)
    {

    }

    QString name() const { return name_; }
    void setName(const QString &value) {
        if (value != name_) {
            name_ = value;
            emit nameChanged();
        }
    }

    qint32 focusMM() const { return focusMM_; }
    void setFocusMM(const qint32 value) {
        if (value != focusMM_) {
            focusMM_ = value;
            emit focusMMChanged();
        }
    }

    qreal lxMM() const { return lxMM_; }
    void setLxMM(const qreal value) {
        if (value != lxMM_) {
            lxMM_ = value;
            emit lxMMChanged();
        }
    }

    qreal lyMM() const { return lyMM_; }
    void setLyMM(const qreal value) {
        if (value != lyMM_) {
            lyMM_ = value;
            emit lyMMChanged();
        }
    }

    qint32 ax() const { return ax_; }
    void setAx(const qint32 value) {
        if (value != ax_) {
            ax_ = value;
            emit axChanged();
        }
    }

    qint32 ay() const { return ay_; }
    void setAy(const qint32 value) {
        if (value != ay_) {
            ay_ = value;
            emit ayChanged();
        }
    }

    QDataStream &Read(QDataStream &stream) {
        stream >> name_ >> focusMM_
               >> lxMM_ >> lyMM_
               >> ax_ >> ay_;
        return stream;
    }

    QDataStream &Write(QDataStream &stream) const {
        stream << name_ << focusMM_
               << lxMM_ << lyMM_
               << ax_ << ay_;
        return stream;
    }


signals:
    void nameChanged();
    void focusMMChanged();
    void lxMMChanged();
    void lyMMChanged();
    void axChanged();
    void ayChanged();

private:
    QString name_;
    qint32 focusMM_ = {};
    qreal lxMM_ = {};
    qreal lyMM_ = {};
    qint32 ax_ = {};
    qint32 ay_ = {};
};

inline QDataStream &operator<<(QDataStream &stream, const QCameraData &cameraData) {
    return cameraData.Write(stream);
}
inline QDataStream &operator>>(QDataStream &stream, QCameraData &cameraData) {
    return cameraData.Read(stream);
}

class QCamerasModel : public QDataItemsModel<QCameraData> {
      Q_OBJECT
  public:
      enum CamerasRoles {
          Name = Qt::UserRole + 1,
          FocusMM,
          LxMM,
          LyMM,
          Ax,
          Ay,
      };

      QCamerasModel(QObject *parent = nullptr) : QDataItemsModel<QCameraData>(parent) { }

      Q_INVOKABLE QCameraData* get(int index) { return dataitems_[index]; }

      QVariant data(const QModelIndex & index, int role) const {
          if (index.row() < 0 || index.row() >= dataitems_.count())
              return QVariant();

          auto &&camera = dataitems_[index.row()];
          switch(role) {
          case Name: return camera->name();
          case FocusMM: return camera->focusMM();
          case LxMM: return camera->lxMM();
          case LyMM: return camera->lyMM();
          case Ax: return camera->ax();
          case Ay: return camera->ay();
          default:
              return QVariant();
          }
      }

      QHash<int, QByteArray> roleNames() const {
          QHash<int, QByteArray> roles;
          roles[Name] = "name";
          roles[FocusMM] = "focusMM";
          roles[LxMM] = "lxMM";
          roles[LyMM] = "lyMM";
          roles[Ax] = "ax";
          roles[Ay] = "ay";
          return roles;
      }
};

inline QDataStream &operator<<(QDataStream &stream, const QCamerasModel &dataitemsModel) {
    return dataitemsModel.Write(stream);
}

inline QDataStream &operator>>(QDataStream &stream, QCamerasModel &dataitemsModel) {
    return dataitemsModel.Read(stream);
}

#endif // CAMERASMODEL_H




