class DronePosition {
   private:
    float latitude_;
    float longitude_;
    float altitude_;

   public:
    bool getPosition();
    float latitude();
    float longitude();
    float altitude();
};