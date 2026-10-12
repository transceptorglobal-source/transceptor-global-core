// ==============================================================================
// TRANSCEPTOR GLOBAL: SPATIAL OPSEC & LOCATION TRACKER KERNEL
// MODULE: ZK LOCATION TRACKER & HAVERSINE ORACLE (CONTOUR 5 / v3.4-ADAPTIVE)
// TIMESTAMP ACCORDING TO JURISDICTIONAL COMPLIANCE: 2026-10-03T12:00:51+03:00
// ==============================================================================

#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

namespace transceptor::spatial {

struct GeoPoint {
    double latitude;  // Широта
    double longitude; // Долгота
};

enum class LocationStatus {
    CONFIRMED_VALID, // Присутствие верифицировано в границах Ойкумены
    PENDING_ANOMALY, // Обнаружены аномалии РЭБ / Спуфинга (K_env filter active)
    REJECTED_OUTSIDE // Физически за пределами геоконтура
};

class ZkLocationTracker {
private:
    constexpr static double EARTH_RADIUS_KM = 6371.0;
    constexpr static double BASE_PRESENCE_DAYS_REQUIRED = 180.0; // 180 дней ценз оседлости

    // Перевод градусов в радианы
    static double to_radians(double degree) {
        return degree * (M_PI / 180.0);
    }

public:
    ZkLocationTracker() = default;

    /**
     * @brief Расчет геодезического расстояния по формуле Гаверсинусов (Haversine)
     */
    static double calculate_haversine_distance_km(const GeoPoint& p1, const GeoPoint& p2) {
        double dLat = to_radians(p2.latitude - p1.latitude);
        double dLon = to_radians(p2.longitude - p1.longitude);

        double lat1Rad = to_radians(p1.latitude);
        double lat2Rad = to_radians(p2.latitude);

        double a = std::sin(dLat / 2.0) * std::sin(dLat / 2.0) +
                   std::sin(dLon / 2.0) * std::sin(dLon / 2.0) * std::cos(lat1Rad) * std::cos(lat2Rad);
        
        double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
        return EARTH_RADIUS_KM * c;
    }

    /**
     * @brief Оценка состояния ценза оседлости с учетом адаптивного фильтра K_env (РЭБ / Спуфинг)
     * @param k_env Коэффициент надежности внешней среды [0.0; 1.0]
     * @param distance_from_center_km Дистанция от центра Ойкумены
     * @param max_allowed_radius_km Допустимый радиус Ойкумены
     */
    LocationStatus evaluate_spatial_presence(double k_env, double distance_from_center_km, double max_allowed_radius_km) {
        // Если K_env < 0.7, активируется режим PENDING (заморозка без сброса)
        if (k_env < 0.7) {
            std::cout << "[ZK_LOCATION] [WARN] Environmental anomaly detected (K_env = " << k_env 
                      << "). Status set to PENDING_ANOMALY." << std::endl;
            return LocationStatus::PENDING_ANOMALY;
        }

        if (distance_from_center_km <= max_allowed_radius_km) {
            return LocationStatus::CONFIRMED_VALID;
        }

        return LocationStatus::REJECTED_OUTSIDE;
    }
};

} // namespace transceptor::spatial