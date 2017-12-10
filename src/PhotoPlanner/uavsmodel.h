#ifndef UAVSMODEL_H
#define UAVSMODEL_H

#include "dataitemsmodel.h"

class QUavData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(qint32 flightTimeMinutes READ flightTimeMinutes WRITE setFlightTimeMinutes NOTIFY flightTimeMinutesChanged)
    Q_PROPERTY(qint32 flightSpeedMPerS READ flightSpeedMPerS WRITE setFlightSpeedMPerS NOTIFY flightSpeedMPerSChanged)
    Q_PROPERTY(qreal maxRollDeg READ maxRollDeg WRITE setMaxRollDeg NOTIFY maxRollDegChanged)
    Q_PROPERTY(qint32 commRadiusKm READ commRadiusKm WRITE setCommRadiusKm NOTIFY commRadiusKmChanged)

public:
    QUavData(QObject *parent = nullptr) : QObject(parent) {}
    QUavData(const QString &name, qint32 flightTimeMinutes, qint32 flightSpeedMPerS, qreal maxRollDeg, qint32 commRadiusKm, QObject *parent)
        : QObject(parent)
        , name_(name)
        , flightTimeMinutes_(flightTimeMinutes)
        , flightSpeedMPerS_(flightSpeedMPerS)
        , maxRollDeg_(maxRollDeg)
        , commRadiusKm_(commRadiusKm)
    {
    }

    QString name() const { return name_; }
    void setName(const QVariant &value) { setName(value.toString()); }
    void setName(const QString &value) {
        if (value != name_) {
            name_ = value;
            emit nameChanged();
        }
    }

    qint32 flightTimeMinutes() const { return flightTimeMinutes_; }
    void setFlightTimeMinutes(const QVariant &value) { setFlightTimeMinutes(value.toUInt()); }
    void setFlightTimeMinutes(const qint32 value) {
        if (value != flightTimeMinutes_) {
            flightTimeMinutes_ = value;
            emit flightTimeMinutesChanged();
        }
    }

    qint32 flightSpeedMPerS() const { return flightSpeedMPerS_; }
    void setFlightSpeedMPerS(const QVariant &value) { setFlightSpeedMPerS(value.toUInt()); }
    void setFlightSpeedMPerS(const qint32 value) {
        if (value != flightSpeedMPerS_) {
            flightSpeedMPerS_ = value;
            emit flightSpeedMPerSChanged();
        }
    }

    qreal maxRollDeg() const { return maxRollDeg_; }
    void setMaxRollDeg(const QVariant &value) { setMaxRollDeg(value.toReal()); }
    void setMaxRollDeg(const qreal value) {
        if (value != maxRollDeg_) {
            maxRollDeg_ = value;
            emit maxRollDegChanged();
        }
    }

    qint32 commRadiusKm() const { return commRadiusKm_; }
    void setCommRadiusKm(const QVariant &value) { setCommRadiusKm(value.toUInt()); }
    void setCommRadiusKm(const qint32 value) {
        if (value != commRadiusKm_) {
            commRadiusKm_ = value;
            emit commRadiusKmChanged();
        }
    }

    QDataStream &Read(QDataStream &stream) {
        stream >> name_ >> flightTimeMinutes_
               >> flightSpeedMPerS_ >> maxRollDeg_
               >> commRadiusKm_;
        return stream;
    }

    QDataStream &Write(QDataStream &stream) const {
        stream << name_ << flightTimeMinutes_
               << flightSpeedMPerS_ << maxRollDeg_
               << commRadiusKm_;
        return stream;
    }

signals:
    void nameChanged();
    void flightTimeMinutesChanged();
    void flightSpeedMPerSChanged();
    void maxRollDegChanged();
    void commRadiusKmChanged();

private:
    QString name_;
    qint32 flightTimeMinutes_ = {};
    qint32 flightSpeedMPerS_ = {};
    qreal maxRollDeg_ = {};
    qint32 commRadiusKm_ = {};
};

inline QDataStream &operator<<(QDataStream &stream, const QUavData &uavData) {
    return uavData.Write(stream);
}
inline QDataStream &operator>>(QDataStream &stream, QUavData &uavData) {
    return uavData.Read(stream);
}

class QUavsModel : public QDataItemsModel<QUavData> {
      Q_OBJECT
  public:
      enum UavsRoles {
          Name = Qt::UserRole + 1,
          FlightTimeMinutes,
          FlightSpeedMPerS,
          MaxRollDeg,
          CommRadiusKm,
      };

      QUavsModel(QObject *parent = nullptr) : QDataItemsModel<QUavData>(parent) { }

      Q_INVOKABLE QUavData* get(int index) { return dataitems_[index]; }

      QVariant data(const QModelIndex & index, int role) const {
          if (index.row() < 0 || index.row() >= dataitems_.count())
              return QVariant();

          auto &&uavdata = dataitems_[index.row()];
          switch(role) {
          case Name: return uavdata->name();
          case FlightTimeMinutes: return uavdata->flightTimeMinutes();
          case FlightSpeedMPerS: return uavdata->flightSpeedMPerS();
          case MaxRollDeg: return uavdata->maxRollDeg();
          case CommRadiusKm: return uavdata->commRadiusKm();
          default:
              return QVariant();
          }
      }

      bool setData(const QModelIndex &index, const QVariant &value, int role) {
          if (index.row() < 0 || index.row() >= dataitems_.count())
              return false;

          auto &&uavdata = dataitems_[index.row()];
          switch(role) {
              case Name: uavdata->setName(value); break;
              case FlightTimeMinutes: uavdata->setFlightTimeMinutes(value); break;
              case FlightSpeedMPerS: uavdata->setFlightSpeedMPerS(value); break;
              case MaxRollDeg: uavdata->setMaxRollDeg(value); break;
              case CommRadiusKm: uavdata->setCommRadiusKm(value); break;
              default:
                  return false;
          }

          emit dataChanged(index, index);
          return true;
      }


      QHash<int, QByteArray> roleNames() const {
          QHash<int, QByteArray> roles;
          roles[Name] = "name";
          roles[FlightTimeMinutes] = "flightTimeMinutes";
          roles[FlightSpeedMPerS] = "flightSpeedMPerS";
          roles[MaxRollDeg] = "maxRollDeg";
          roles[CommRadiusKm] = "commRadiusKm";
          return roles;
      }

    public slots:
      void OnNameChanged() {
          auto uavData = static_cast<QUavData *>(sender());
          int index = dataitems_.indexOf(uavData);

          auto modelIndex = createIndex(index, index);
          emit dataChanged(modelIndex, modelIndex);
      }

};

inline QDataStream &operator<<(QDataStream &stream, const QUavsModel &dataitemsModel) {
    return dataitemsModel.Write(stream);
}

inline QDataStream &operator>>(QDataStream &stream, QUavsModel &dataitemsModel) {
    return dataitemsModel.Read(stream);
}


#endif // UAVSMODEL_H
